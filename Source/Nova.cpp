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
#include <SDL2\SDL.h>

#include "Shader.h"

#include <math.h>
#include <vector>

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

		/* FORWARD RENDERERING SHADER*/
		ShaderProgram* forwardRender = new ShaderProgram("forward_render.glsl");
		forwardRender->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Forward/forward_render.fs"));
		forwardRender->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Forward/forward_render.vs"));
		ResourceManager::CacheShaderProgram(forwardRender);
		ShaderBinder::BindShaderProgram("forward_render.glsl");
		forwardRender->SetUniform1i("numPointLights", pLightCount);
		forwardRender->SetUniform1i("numSpotLights", sLightCount);
		forwardRender->SetUniform1i("numDirLights", dLightCount);
		ShaderBinder::UnbindShaderProgram();

		/* new deferred renderer */
		{
			/*geometry pass*/
			if (m_geometryPass.Init())
			{
				m_geometryPass.SetDiffuseTextureUnit(0);
				m_geometryPass.SetNormalTextureUnit(1);

				Logger::InfoBlock("Deferred Renderer", "Info: geometry pass initialised", true);
			}
			else
			{
				Logger::ErrorBlock("Geometry Pass Error", "Error: the geometry pass could not be intitialised", true);
			}

			/*Stencil pass*/
			if (m_stencilPass.Init())
			{
				Logger::InfoBlock("Deferred Renderer", "Info: stencil pass initialised", true);
			}
			else
			{
				Logger::ErrorBlock("Stencil Pass Error", "Error: the stencil pass could not be intitialised", true);
			}

			/*point light pass*/
			if (m_pointLightPass.Init())
			{
				m_pointLightPass.SetPositionTextureUnit(m_gBuffer.GB_POSITION);
				m_pointLightPass.SetDiffuseTextureUnit(m_gBuffer.GB_ALBEDOSPEC);
				m_pointLightPass.SetNormalTextureUnit(m_gBuffer.GB_NORMAL);
				m_pointLightPass.SetScreenSize(m_window->GetWidth(), m_window->GetHeight());

				Logger::InfoBlock("Deferred Renderer", "Info: point light pass initialised", true);
			}
			else
			{
				Logger::ErrorBlock("Point Light Pass Error", "Error: point light pass could not be initialised", true);
			}

			/*directional light pass*/
			if (m_dirLightPass.Init())
			{
				m_dirLightPass.SetPositionTextureUnit(m_gBuffer.GB_POSITION);
				m_dirLightPass.SetDiffuseTextureUnit(m_gBuffer.GB_ALBEDOSPEC);
				m_dirLightPass.SetNormalTextureUnit(m_gBuffer.GB_NORMAL);
				m_dirLightPass.SetScreenSize(m_window->GetWidth(), m_window->GetHeight());

				Logger::InfoBlock("Deferred Renderer", "Info: directional light pass initialised", true);
			}
			else
			{
				Logger::ErrorBlock("Directional Light Pass Error", "Error: directional light pass could not be initialised", true);
			}

			/* spot light pass*/
			if (m_spotLightPass.Init())
			{
				m_spotLightPass.SetPositionTextureUnit(m_gBuffer.GB_POSITION);
				m_spotLightPass.SetDiffuseTextureUnit(m_gBuffer.GB_ALBEDOSPEC);
				m_spotLightPass.SetNormalTextureUnit(m_gBuffer.GB_NORMAL);
				m_spotLightPass.SetScreenSize(m_window->GetWidth(), m_window->GetHeight());

				Logger::InfoBlock("Deferred Renderer", "Info: spot light pass initialised", true);
			}
			else
			{
				Logger::ErrorBlock("Spot Light Pass Error", "Error: spot light pass could not be initialised", true);
			}

			/* skybox pass */
			if (m_skyboxPass.Init())
			{
				m_skyboxPass.SetCubeMapTextureUnit(0);
				Logger::InfoBlock("Deferred Renderer", "Info: skybox pass initialised", true);
			}
			else
			{
				Logger::ErrorBlock("Spot Light Pass Error", "Error: skybox pass could not be initialised", true);
			}

		}

		CacheResources();

		SetUpLights();

		m_gBuffer.Init(Window::GetWidth(), Window::GetHeight());

		InitFullScreenQuad();

		m_sceneManager.AddScene(new ScratchPad::SpaceScene());
		m_sceneManager.AddScene(new ScratchPad::MaterialTestScene());
		m_sceneManager.AddScene(new ScratchPad::ECSTestScene(m_geometryPass));

		Profiler::ProfileManager::End("Initialisation");
		Profiler::ProfileManager::m_FPS = 60;
		Profiler::ProfileManager::Display();

//		m_entityManager.AddSystem(ECS::System(m_entityManager));

		Run();
	}

	void Nova::CacheResources()
	{
		CacheMeshes();
		CacheTextures();
	}

	void Nova::CacheMeshes()
	{
		ResourceManager::CacheIndexedMesh("cubetest","Models/shapes/cube.obj");
		ResourceManager::CacheIndexedMesh("sphere", "Models/sphere/sphere.obj");
		ResourceManager::CacheIndexedMesh("pointLightSphereVolume", new Sphere(1, 10, 10));

		m_sphere = ResourceManager::GetIndexedMesh("sphere");
		//m_sphere = ResourceManager::GetIndexedMesh("pointLightSphereVolume");
	}

	void Nova::CacheTextures()
	{
	
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

		Profiler::ProfileManager::Begin("GPass");
		DSGeometryPass();
		Profiler::ProfileManager::End("GPass");
	
		/* POINT LIGHT PASS*/
		Profiler::ProfileManager::Begin("PLightPass");
		glEnable(GL_STENCIL_TEST);
		for (unsigned int i = 0; i < pLightCount; i++)
		{
			DSStencilPass(i);
			DSPointLightPass(i);
		}
		glDisable(GL_STENCIL_TEST);
		Profiler::ProfileManager::End("PLightPass");
		////////////////////

		/* DIRECTIONAL LIGHT PASS*/
		Profiler::ProfileManager::Begin("DLightPass");
		DSDirectionalLightPass();
		Profiler::ProfileManager::End("DLightPass");
		//////////////////////////

		/* SKYBOX PASS */
		Profiler::ProfileManager::Begin("SPass");
		DSSkyboxPass();
		Profiler::ProfileManager::End("SPass");
		//////////////////////////

		/* FINAL PASS*/
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
			transform.Reset();

			m_sphere->DrawBegin(DrawMode::NOVA_WIREFRAME);
			for (int i = 0; i < pLightCount; i++)
			{
				transform.GetPos() = pLights[i].position;
				transform.GetScale() = glm::vec3(Lights::CalcPointLightBSphere(pLights[i]));
				m_geometryPass.SetMVP(transform.GetMVP(m_sceneManager.GetActiveScene()->GetActiveCamera().GetViewProject()));
				m_geometryPass.SetModel(transform.GetModel());

				m_sphere->DrawGroup();
			}
			m_sphere->DrawEnd();
		  }


		glDepthMask(GL_FALSE);
	}


	void Nova::DSStencilPass(unsigned int i)
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

		Transform t;
		
		t.GetPos()    = pLights[i].position;
		t.GetScale()  = glm::vec3(Lights::CalcPointLightBSphere(pLights[i]));

		glm::mat4 mvp = m_sceneManager.GetActiveScene()->GetActiveCamera().GetViewProject() * t.GetModel();
		m_stencilPass.SetMVP(mvp);
	
		m_sphere->Draw();

		/*POINT LIGHT PASS*/
		m_gBuffer.BindForLightPass();
		
		m_pointLightPass.Enable();
		m_pointLightPass.SetMatSpecularPower(200);
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

	void Nova::DSPointLightPass(unsigned int i)
	{
		
	}

	void Nova::DSDirectionalLightPass()
	{
		m_gBuffer.BindForLightPass();
		m_dirLightPass.Enable();
		m_dirLightPass.SetMatSpecularPower(70);
		m_dirLightPass.SetEyeWorldPos(m_sceneManager.GetActiveScene()->GetActiveCamera().GetPosition());

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		Transform t;

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

		GLuint width = Window::GetWidth();
		GLuint height = Window::GetHeight();
		
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

		Transform t;

		t.GetScale() = glm::vec3(500, 500, 500);

		m_skyboxPass.Enable();

		glActiveTexture(GL_TEXTURE0);
		const GLuint id = m_sceneManager.GetActiveScene()->GetSkyTexture()->id;
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		m_skyboxPass.SetMVP(m_sceneManager.GetActiveScene()->GetActiveCamera().GetViewProject() * t.GetModel());

		m_sphere->Draw();
		glDepthFunc(GL_LESS);

		glCullFace(OldCullFaceMode);
		glDepthFunc(OldDepthFuncMode);
	}

	void Nova::ForwardRender()
	{
		///* test directinoal light */
		//////////////////////////////
		//DirectionalLight light;
		//light.base.colour = Utils::Convert_RGB_ToGLSLColorSpace(glm::vec3(255, 255, 200));
		//light.base.ambientIntensity = 0.01f;
		//light.base.diffuseIntensity = 1;
		//light.direction = glm::vec3(0, cos(j), sin(j));
		//////////////////////////////

		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ShaderProgram* triangleProgram = ResourceManager::GetShaderProgram("forward_render.glsl")->Bind();
		//{
		//	// camera
		//	triangleProgram->SetUniform3f("eyeWorldPos", cam.GetPosition());

		//	// material
		//	triangleProgram->SetUniform1f("material.matSpecularIntensity", 1);
		//	triangleProgram->SetUniform1f("material.matSpecularPower", 400);


		//	{
		//		// point lights
		//		for (unsigned int i = 0; i < 50; i++)
		//		{

		//			std::string loc = "pLights[" + std::to_string(i) + "]";

		//			triangleProgram->SetUniform3f(loc + ".base.colour", pLights[i].base.colour);
		//			triangleProgram->SetUniform1f(loc + ".base.ambientIntensity", pLights[i].base.ambientIntensity);
		//			triangleProgram->SetUniform1f(loc + ".base.diffuseIntensity", pLights[i].base.diffuseIntensity);
		//			triangleProgram->SetUniform3f(loc + ".position", pLights[i].position);
		//			triangleProgram->SetUniform1f(loc + ".atten.constant", pLights[i].atten.constant);
		//			triangleProgram->SetUniform1f(loc + ".atten.linear", pLights[i].atten.linear);
		//			triangleProgram->SetUniform1f(loc + ".atten.exp", pLights[i].atten.exp);
		//		}

		//		// spot lights
		//		for (unsigned int i = 0; i < 50; i++)
		//		{
		//			std::string loc = "sLights[" + std::to_string(i) + "]";
		//			triangleProgram->SetUniform3f(loc + ".direction", sLights[i].direction);
		//			triangleProgram->SetUniform1f(loc + ".cutoff", sLights[i].cutoff);
		//			triangleProgram->SetUniform3f(loc + ".base.base.colour", sLights[i].base.base.colour);
		//			triangleProgram->SetUniform1f(loc + ".base.base.ambientIntensity", sLights[i].base.base.ambientIntensity);
		//			triangleProgram->SetUniform1f(loc + ".base.base.diffuseIntensity", sLights[i].base.base.diffuseIntensity);
		//			triangleProgram->SetUniform3f(loc + ".base.position", sLights[i].base.position);
		//			triangleProgram->SetUniform1f(loc + ".base.atten.constant", sLights[i].base.atten.constant);
		//			triangleProgram->SetUniform1f(loc + ".base.atten.linear", sLights[i].base.atten.linear);
		//			triangleProgram->SetUniform1f(loc + ".base.atten.exp", sLights[i].base.atten.exp);
		//		}

		//		// directional lights
		//		{
		//			for (unsigned int i = 0; i < 1; i++)
		//			{
		//				std::string loc = "dLights[" + std::to_string(i) + "]";
		//				triangleProgram->SetUniform3f(loc + ".base.colour", light.base.colour);
		//				triangleProgram->SetUniform1f(loc + ".base.ambientIntensity", light.base.ambientIntensity);
		//				triangleProgram->SetUniform1f(loc + ".base.diffuseIntensity", light.base.diffuseIntensity);
		//				triangleProgram->SetUniform3f(loc + ".direction", light.direction);
		//			}
		//		}

		//	}

		//	transform.Reset();

		//	static const GLTexture* startwarstex = ResourceManager::GetTexture("Models/starwars/Arc170_blinn1.png");

		//	glBindTexture(GL_TEXTURE_2D, startwarstex->id);
		//	for (int i = 0; i < 10; i++)
		//	{
		//		for (int j = 0; j < 10; j++)
		//		{
		//			transform.GetPos() = glm::vec3(i * 4 + 5, 0, j * 4 + 5);
		//			transform.GetScale() = glm::vec3(0.001, 0.001, 0.001);
		//			triangleProgram->SetUniform4m("MVP", cam.GetViewProject() * transform.GetModel());
		//			triangleProgram->SetUniform4m("Model", transform.GetModel());
		//			ResourceManager::GetIndexedMesh("Models/starwars/ARC170.3DS")->Draw();
		//		}
		//	}

		//	static const GLTexture* moon = ResourceManager::GetTexture("Models/planets/moon/MoonMap2_2500x1250.png");

		//	glBindTexture(GL_TEXTURE_2D, moon->id);

		//	transform.Reset();

		//	transform.GetScale() = glm::vec3(0.1, 0.1, 0.1);
		//	transform.GetPos() = glm::vec3(25, 10, 25);

		//	triangleProgram->SetUniform4m("MVP", cam.GetViewProject() * transform.GetModel());
		//	triangleProgram->SetUniform4m("Model", transform.GetModel());

		//	ResourceManager::GetIndexedMesh("Models/planets/moon/moon.obj")->Draw();
		//}
	}
}


