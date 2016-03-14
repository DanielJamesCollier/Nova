#include "Nova.h"
#include "Window.h"
#include "ResourceManager.h"
#include "ShaderBinder.h"
#include "GLTexture.h"
#include "Utils.h"
#include "Material.h"
#include "Sphere.h"
#include "Logger.h"
#include "MathUtil.h"
#include "SpaceScene.h"
#include "ECSTestScene.h"
#include "MaterialTestScene.h"
#include "ProfileManager.h"
#include "Shader.h"
#include <SDL2\SDL.h>
#include <math.h>
#include <vector>
#include "TextureBinder.h"

namespace Nova
{
	Nova::Nova(Window* window)
	{
		m_window = window;
	}

	Nova::~Nova()
	{
		ResourceManager::Dispose();
		delete m_window;
		glDeleteVertexArrays(1, &m_vaoQuad);
	}

	void Nova::Start()
	{
		if (m_running) return;
		m_running = true;

		Profiler::ProfileManager::Begin("Initialisation");

		ResourceManager::Init();
		InputManager::Init();

		///* FORWARD RENDERERING SHADER*/
		//ShaderProgram* forwardRender = new ShaderProgram("forward_render.glsl");
		//forwardRender->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Forward/forward_render.fs"));
		//forwardRender->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Forward/forward_render.vs"));
		//ResourceManager::CacheShaderProgram(forwardRender);
		//ShaderBinder::BindShaderProgram("forward_render.glsl");
		//forwardRender->SetUniform1i("numPointLights", pLightCount);
		//forwardRender->SetUniform1i("numSpotLights", sLightCount);
		//forwardRender->SetUniform1i("numDirLights", dLightCount);
		//ShaderBinder::UnbindShaderProgram();

		/* new deferred renderer */
		Logger& log = Logger::GetInstance();
		log.InfoBlockBegin("Deferred Renderer Initialisation");
		{
			/////////////////////////////
			if (m_geometryPass.Init())
			{
				log.InfoBlockMessage("Info: geometry pass initialised\n");
			}
			else
			{
				log.InfoBlockMessage("Error: geometry pass did not initialise\n");
			}

			/////////////////////////////
			if (m_stencilPass.Init())
			{
				log.InfoBlockMessage("Info: stencil pass initialised\n");
			}
			else
			{
				log.InfoBlockMessage("Error: stencil pass did not initialise\n");
			}

		
			/////////////////////////////
			if (m_pointLightPass.Init())
			{
				m_pointLightPass.SetScreenSize(m_window->GetWidth(), m_window->GetHeight());
				log.InfoBlockMessage("Info: point light pass initialised\n");
			}
			else
			{
				log.InfoBlockMessage("Info: point light pass not initialised\n");
			}

			/////////////////////////////
			if (m_dirLightPass.Init())
			{
				m_dirLightPass.SetScreenSize(m_window->GetWidth(), m_window->GetHeight());
				log.InfoBlockMessage("Info: directional light pass initialised\n");
			}
			else
			{
				log.InfoBlockMessage("Info: directional light pass not initialised\n");
			}

			/////////////////////////////
			if (m_spotLightPass.Init())
			{
				m_spotLightPass.SetScreenSize(m_window->GetWidth(), m_window->GetHeight());
				log.InfoBlockMessage("Info: spot light pass initialised\n");
			}
			else
			{
				log.InfoBlockMessage("Info: spot light pass not initialised\n");
			}

			/////////////////////////////
			if (m_skyboxPass.Init())
			{
				m_skyboxPass.SetCubeMapTextureUnit(0);
				log.InfoBlockMessage("Info: skybox pass initialised\n");
			}
			else
			{
				log.InfoBlockMessage("Info: skybox pass not initialised\n");
			}
		}
		log.InfoBlockEnd();

		CacheResources();
		SetUpLights();

		m_gBuffer.Init(Window::GetWidth(), Window::GetHeight());

		InitFullScreenQuad();

		m_sceneManager.AddScene(new ScratchPad::SpaceScene());
	  //m_sceneManager.AddScene(new ScratchPad::MaterialTestScene());
		m_sceneManager.AddScene(new ScratchPad::ECSTestScene(m_geometryPass));

		Profiler::ProfileManager::End("Initialisation");
		Profiler::ProfileManager::m_FPS = 60;
		Profiler::ProfileManager::Display();
		Run();
	}

	void Nova::CacheResources()
	{
		CacheMeshes();
	}

	void Nova::CacheMeshes()
	{
		ResourceManager::CacheIndexedMesh("cube", "Models/shapes/Crate1.obj");
		ResourceManager::CacheIndexedMesh("sphere", "Models/sphere/sphere.obj");
		ResourceManager::CacheIndexedMesh("pointLightSphereVolume", new Sphere(1, 10, 10));

		m_sphere = ResourceManager::GetIndexedMesh("cube");
	}

	void Nova::InitFullScreenQuad()
	{
		glGenVertexArrays(1, &m_vaoQuad);
	}

	void Nova::SetUpLights()
	{
		for (int i = 0; i < pLightCount; i++)
		{
			glm::vec3 position = glm::vec3(rand() % 120, 0.0f, rand() % 120);

			pLights[i] = PointLight(position, Attenuation(1.0f, 100.0f, 0.1f), BaseLight(glm::vec3(rand() % 255, rand() % 255, rand() % 255), 0.0f, 1.0f));
		}

		for (unsigned int i = 0; i < sLightCount; i++)
		{
			sLights[i] = SpotLight(PointLight(glm::vec3(rand() % 50, 5.0f, rand() % 50), Attenuation(1.0f, 0.001f, 10.0f), BaseLight(glm::vec3(rand() % 255, rand() % 255, rand() % 255), 0, 0.3)), glm::vec3(0.0f, -1.0f, 0.0f));
		}

		for (unsigned int i = 0; i < dLightCount; i++)
		{
			dLights[i] = DirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), BaseLight(Utils::Convert_RGB_ToGLSLColorSpace(glm::vec3(255, 255, 200)), 0.0f, 1.0f));
		}
	}

	void Nova::Stop()
	{
		if (!m_running) return;
		m_running = false;

		ResourceManager::Dispose();
	}

	void Nova::Run()
	{
		int frameCount = 0;
		int currentTime = SDL_GetTicks();
		int startTime = currentTime;
		int passedTime = currentTime;
		int startTimeDelta = currentTime;

		// calculate fps
		while (m_running)
		{
			Profiler::ProfileManager::Begin("Run");
			{
				currentTime = SDL_GetTicks();
				passedTime = currentTime - startTime;

				frameCount++;


				Profiler::ProfileManager::Begin("ProcessInput");
				{
					ProcessInput();
				}
				Profiler::ProfileManager::End("ProcessInput");

				Profiler::ProfileManager::Begin("Update");
				{
					Update((currentTime - startTimeDelta) * 0.001f);
					startTimeDelta = currentTime;
				}
				Profiler::ProfileManager::End("Update");

				Profiler::ProfileManager::Begin("Render");
				{
					Render();
				}
				Profiler::ProfileManager::End("Render");


			}
			Profiler::ProfileManager::End("Run");

			if (passedTime >= 1000)
			{
				Profiler::ProfileManager::m_FPS = frameCount;
				startTime = currentTime;
				frameCount = 0;
				std::cout << "GBuffer - TextureBinds: " << m_gBuffer.textureBindsPerFrame << std::endl;
				m_gBuffer.textureBindsPerFrame = 0;
				Profiler::ProfileManager::Display();
			}
		}
		Stop();
	}

	void Nova::ProcessInput()
	{
		m_inputManager.Update(m_running);
		m_sceneManager.ProcessInput();

		if (InputManager::IsKeyPressed(SDLK_p))
		{
			m_displayVolumes = true;
		}
		else if (InputManager::IsKeyPressed(SDLK_o))
		{
			m_displayVolumes = false;
		}

		if (InputManager::IsKeyPressed(SDLK_v))
		{
			m_srgb = true;
		}
		else if (InputManager::IsKeyPressed(SDLK_c))
		{
			m_srgb = false;
		}
	}

	void Nova::Update(float delta)
	{
		m_sceneManager.UpdateCurrent(delta);
	}

	void Nova::Render()
	{
		j += 0.001f;

		Profiler::ProfileManager::Begin("DeferredRender");
		DSRenderScene();
		Profiler::ProfileManager::End("DeferredRender");

		Profiler::ProfileManager::Begin("BackBuffer");
		m_window->SwapBackBuffer();
		Profiler::ProfileManager::End("BackBuffer");
	}

	/* refactored deferred renderer */
	void Nova::DSRenderScene()
	{
		m_gBuffer.StartFrame();

		/* GEOMETRY PASS */
		//////////////////////////
		Profiler::ProfileManager::Begin("GPass");
		DSGeometryPass();
		Profiler::ProfileManager::End("GPass");
		//////////////////////////

		/* POINT LIGHT PASS*/
		//////////////////////////
		Profiler::ProfileManager::Begin("PLightPass");
		glEnable(GL_STENCIL_TEST);
		for (unsigned int i = 0; i < pLightCount; i++)
		{
			DSPointLightStencilPass(i);
		}
		glDisable(GL_STENCIL_TEST);
		Profiler::ProfileManager::End("PLightPass");
		////////////////////

		/* DIRECTIONAL LIGHT PASS*/
		//////////////////////////
		Profiler::ProfileManager::Begin("DLightPass");
		DSDirectionalLightPass();
		Profiler::ProfileManager::End("DLightPass");
		//////////////////////////

		/* SKYBOX PASS */
		//////////////////////////
		Profiler::ProfileManager::Begin("SPass");
		DSSkyboxPass();
		Profiler::ProfileManager::End("SPass");
		//////////////////////////

		/* FINAL PASS*/
		//////////////////////////
		Profiler::ProfileManager::Begin("FPass");
		DSFinalPass();
		Profiler::ProfileManager::End("FPass");
		//////////////////////////

	}

	void Nova::DSGeometryPass()
	{
		m_geometryPass.Enable();

		m_gBuffer.BindForGeomPass();

		// Only the geometry pass updates the depth buffer
		glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		m_sceneManager.RenderCurrent(&m_geometryPass);

		if (m_displayVolumes)
		{
			m_transform.Reset();

			m_sphere->DrawBegin(DrawMode::NOVA_WIREFRAME);
			for (int i = 0; i < pLightCount; i++)
			{
				m_transform.SetPosition(pLights[i].position);
				m_transform.SetScale(glm::vec3(Lights::CalcPointLightBSphere(pLights[i])));
				m_geometryPass.SetMVP(m_transform.GetMVP(m_sceneManager.GetActiveScene()->GetActiveCamera().GetViewProject()));
				m_geometryPass.SetModel(m_transform.GetModel());

				m_sphere->DrawGroup();
			}
			m_sphere->DrawEnd();
		}


		glDepthMask(GL_FALSE);
	}


	void Nova::DSPointLightStencilPass(unsigned int i)
	{
		/*STENCIL PASS*/
		m_stencilPass.Enable();
		
		m_gBuffer.BindForStencilPass();
		glEnable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);

		glClear(GL_STENCIL_BUFFER_BIT);

		// We need the stencil test to be enabled but we want it
		// to succeed always. Only the depth test matters.
		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		m_transform.Reset(); // expensive

		m_transform.SetPosition(pLights[i].position); // expensive
		m_transform.SetScale(glm::vec3(Lights::CalcPointLightBSphere(pLights[i]))); // expensive

		glm::mat4 mvp = m_sceneManager.GetActiveScene()->GetActiveCamera().GetViewProject() * m_transform.GetModel(); // expensive
		m_stencilPass.SetMVP(mvp);

		m_sphere->Draw();

		/*POINT LIGHT PASS*/
		
		m_gBuffer.BindForLightPass();

		m_pointLightPass.Enable();
		m_pointLightPass.SetMatSpecularPower(100);
		m_pointLightPass.SetEyeWorldPos(m_sceneManager.GetActiveScene()->GetActiveCamera().GetPosition());

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		// update uniforms
		m_pointLightPass.SetPointLight(pLights[i]);
		m_pointLightPass.SetMVP(mvp);

		m_sphere->Draw();

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}

	void Nova::DSDirectionalLightPass()
	{
		m_gBuffer.BindForLightPass();
		m_dirLightPass.Enable();
		m_dirLightPass.SetMatSpecularPower(200);
		m_dirLightPass.SetEyeWorldPos(m_sceneManager.GetActiveScene()->GetActiveCamera().GetPosition());

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		m_transform.Reset();

		glBindVertexArray(m_vaoQuad);
		for (unsigned int i = 0; i < dLightCount; i++)
		{
			/* animation code */
			dLights[i].direction = glm::vec3(0, cos(j * 5), sin(j * 5));
			////////////////////

			m_dirLightPass.SetDirectionalLight(dLights[i]);
			m_dirLightPass.SetMVP(glm::mat4());

			// draw quad using the geometry shader
			glDrawArrays(GL_POINTS, 0, 1);
		}
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}

	void Nova::DSFinalPass()
	{
		m_gBuffer.BindForFinalPass();

		static const GLuint width = Window::GetWidth();
		static const GLuint height = Window::GetHeight();

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	void Nova::DSSpotLightPass()
	{

		//Transform t;

		//m_spotLightPass.Enable();

		//static const IndexedMesh* sLightVolume = ResourceManager::GetIndexedMesh("cube");
		//t.GetScale() = glm::vec3(5, 5, 5);

		//// get mesh

		//m_spotLightPass.SetMatSpecularPower(400);
		//m_spotLightPass.SetEyeWorldPos(cam.GetPosition());

		//sLightVolume->DrawBegin();
		//for (unsigned int i = 0; i < sLightCount; ++i)
		//{
		//	/* animation code */
		//	//float sphereScale = Lights::Lights::CalcPointLightBSphere2(pLights[i]); // wtf m8 
		//	glm::vec3& p = sLights[i].base.position;
		//	t.GetPos() = p;
		//	////////////////////

		//	// update uniforms
		//	m_spotLightPass.SetSpotLight(sLights[i]);

		//	m_pointLightPass.SetMVP(cam.GetViewProject() * t.GetModel());

		//	// draw sphere light volume
		//	sLightVolume->DrawGroup();
		//}
		//sLightVolume->DrawEnd();

	}

	void Nova::DSSkyboxPass()
	{
		glEnable(GL_DEPTH_TEST);
		GLint OldCullFaceMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
		GLint OldDepthFuncMode;
		glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_transform.Reset();

		m_transform.SetScale(glm::vec3(500, 500, 500));

		m_skyboxPass.Enable();

		GLTexture* skybox = m_sceneManager.GetActiveScene()->GetSkyTexture();
		TextureBinder::GetInstance().BindTexture(0, skybox);

		m_skyboxPass.SetMVP(m_sceneManager.GetActiveScene()->GetActiveCamera().GetViewProject() * m_transform.GetModel());

		m_sphere->Draw();
		glDepthFunc(GL_LESS);

		glCullFace(OldCullFaceMode);
		glDepthFunc(OldDepthFuncMode);
	}
}



