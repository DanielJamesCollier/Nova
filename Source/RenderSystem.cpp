#include "RenderSystem.h"
#include "Logger.h"

namespace Nova
{
	namespace ECS
	{
		RenderSystem::RenderSystem(StaticPool<Component::CTransform>& transformComps, StaticPool<Component::CRenderable>& renderComps)
			:
			m_mesh(MAX_MESHES),
			m_material(MAX_MATERIALS),
			m_pointLights(MAX_LIGHTS_POINT),
			m_spotLights(MAX_LIGHTS_SPOT),
			m_cTransforms(transformComps),
			m_cRenderable(renderComps),
			m_meshLoader(MeshLoader::GetInstance())
		{
			m_currentMat = -1;

			glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			ResourceManager::CacheIndexedMesh("cube", "Models/shapes/Crate1.obj");
			ResourceManager::CacheIndexedMesh("sphere", "Models/sphere/sphere.obj");
			ResourceManager::CacheIndexedMesh("pointLightSphereVolume", new Sphere(1, 10, 10));

			m_pointLightStencilVolume = ResourceManager::GetIndexedMesh("sphere");
			m_skyboxCube = ResourceManager::GetIndexedMesh("cube");

			// temp light setup
			for (int i = 0; i < MAX_LIGHTS_POINT; i++)
			{
				glm::vec3 position = glm::vec3(rand() % 120, 0.0f, rand() % 120);

				pLights[i] = PointLight(position, Attenuation(1.0f, 100.0f, 0.1f), BaseLight(glm::vec3(rand() % 255, rand() % 255, rand() % 255), 0.0f, 1.0f));
			}

			for (int i = 0; i < MAX_LIGHTS_SPOT; i++)
			{
				sLights[i] = SpotLight(PointLight(glm::vec3(rand() % 50, 5.0f, rand() % 50), Attenuation(1.0f, 0.001f, 10.0f), BaseLight(glm::vec3(rand() % 255, rand() % 255, rand() % 255), 0, 0.3)), glm::vec3(0.0f, -1.0f, 0.0f));
			}


			dLight = DirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), BaseLight(Utils::Convert_RGB_ToGLSLColorSpace(glm::vec3(255, 255, 200)), 0.0f, 1.0f));

			InitialiseDSPasses();
		}

		RenderSystem::~RenderSystem()
		{
			bool valid;
			for (unsigned int i = 0, size = m_mesh.GetActiveObjectCount(); i < size; ++i)
			{
				IndexedMesh& mesh = m_mesh.GetObjectWithPoolID(i, valid);

				if (valid) mesh.DisposeGLResources();
			}

			for (unsigned int i = 0, size = m_material.GetActiveObjectCount(); i < size; ++i)
			{
				Material& mat = m_material.GetObjectWithPoolID(i, valid);

				if (valid) mat.DisposeGLResources();
			}

			ShaderBinder::UnbindShaderProgram();

			glDeleteVertexArrays(1, &m_vaoQuad);
		}

		void RenderSystem::InitialiseDSPasses()
		{
			/* deferred renderer */
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
				std::cout << "working" << std::endl;
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
					m_pointLightPass.SetScreenSize(Window::GetWidth(), Window::GetHeight());
					log.InfoBlockMessage("Info: point light pass initialised\n");
				}
				else
				{
					log.InfoBlockMessage("Info: point light pass not initialised\n");
				}

				/////////////////////////////
				if (m_dirLightPass.Init())
				{
					m_dirLightPass.SetScreenSize(Window::GetWidth(), Window::GetHeight());
					log.InfoBlockMessage("Info: directional light pass initialised\n");
				}
				else
				{
					log.InfoBlockMessage("Info: directional light pass not initialised\n");
				}

				/////////////////////////////
				if (m_spotLightPass.Init())
				{
					m_spotLightPass.SetScreenSize(Window::GetWidth(), Window::GetHeight());
					log.InfoBlockMessage("Info: spot light pass initialised\n");
				}
				else
				{
					log.InfoBlockMessage("Info: spot light pass not initialised\n");
				}

				/////////////////////////////
				if (m_skyboxPass.Init())
				{
					log.InfoBlockMessage("Info: skybox pass initialised\n");
				}
				else
				{
					log.InfoBlockMessage("Info: skybox pass not initialised\n");
				}
			}
			log.InfoBlockEnd();


			m_gBuffer.Init(Window::GetWidth(), Window::GetHeight());

			InitFullScreenQuad();
		}

		void RenderSystem::InitFullScreenQuad()
		{
			glGenVertexArrays(1, &m_vaoQuad);
		}

		int RenderSystem::AddMesh(const IndexedMesh& mesh)
		{
			return m_mesh.AddObject(mesh);
		}

		int RenderSystem::AddMesh(const std::string& filePath)
		{
			bool valid;

			IndexedMesh mesh = m_meshLoader.LoadIndexedMesh_stack(filePath, valid);

			if (valid)
			{
				int id = m_mesh.AddObject(mesh);
				return id;
			}
			return -1;
		}

		int RenderSystem::AddMaterial(const Material& material)
		{
			int id = m_material.AddObject(material);
			return id;
		}

		void RenderSystem::SetCamera(Camera* camera)
		{
			m_camera = camera;
		}

		void RenderSystem::Update()
		{
			m_gBuffer.StartFrame();

			DSGeometryPass();

			glEnable(GL_STENCIL_TEST);
			for (unsigned int i = 0; i < MAX_LIGHTS_POINT; i++)
			{
				DSPointLightStencilPass(i);
			}
			glDisable(GL_STENCIL_TEST);

			DSDirectionalLightPass();

			DSSkyboxPass();

			DSFinalPass();
		}

		void RenderSystem::SetSkyboxTexture(GLTexture* texture)
		{
			m_skyboxTexture = texture;
		}

		void RenderSystem::DSGeometryPass()
		{
			m_geometryPass.Enable();

			m_gBuffer.BindForGeomPass();

			// Only the geometry pass updates the depth buffer
			glDepthMask(GL_TRUE);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			DSBuildBatches();
			DSDrawBatches();

		
			m_transform.Reset();

			m_pointLightStencilVolume->DrawBegin(DrawMode::NOVA_WIREFRAME);
			for (int i = 0; i < MAX_LIGHTS_POINT; i++)
			{
				m_transform.SetPosition(pLights[i].position);
				m_transform.SetScale(glm::vec3(Lights::CalcPointLightBSphere(pLights[i])));
				m_geometryPass.SetMVP(m_transform.GetMVP(m_camera->GetViewProject()));
				m_geometryPass.SetModel(m_transform.GetModel());
				m_pointLightStencilVolume->DrawGroup();
			}
			m_pointLightStencilVolume->DrawEnd();

			glDepthMask(GL_FALSE);
		}

		void RenderSystem::DSBuildBatches()
		{
			unsigned int r_poolSize = m_cRenderable.GetActiveObjectCount();
			std::unordered_map<unsigned int, unsigned int> m_meshToIndex;
			unsigned int index = 0;

			bool valid = false;

			for (unsigned int i = 0; i < r_poolSize; ++i)
			{
				unsigned int staticID = 0;
				const Component::CRenderable& rComp = m_cRenderable.GetObjectWithPoolID(i, staticID, valid);
				const Component::CTransform& tComp = m_cTransforms.GetObjectWithStaticID(staticID, valid);


				if (valid) // if entity has a transform and a renderable comp process it
				{
					// maps mesh id's to an index location in the m_batches vector
					// used to check if a back aready exist for that mesh

					auto search = m_meshToIndex.find(rComp.mesh);

					// if mesh is not found in a batch then create a new batch and add it
					if (search == m_meshToIndex.end())
					{
						RenderBatch batch;
						batch.transform.push_back(tComp);
						batch.material = rComp.material;
						batch.mesh = rComp.mesh;
						m_batches.push_back(batch);
						m_meshToIndex.insert(std::make_pair(batch.mesh, index));
						index++;
					}
					else // // the mesh was found in a batch - add another transform to that batch
					{
						if (m_batches[search->second].material == rComp.material)
						{
							m_batches[search->second].transform.push_back(tComp);
						}
						else
						{
							RenderBatch batch;
							batch.transform.push_back(tComp);
							batch.material = rComp.material;
							batch.mesh = rComp.mesh;
							m_batches.push_back(batch);
							m_meshToIndex.insert(std::make_pair(batch.mesh, index++));
						}
					}
				}
			}
		}

		void RenderSystem::DSDrawBatches()
		{
			unsigned int r_poolSize = m_batches.size();
			//std::cout << "number of batches: " << r_poolSize << std::endl;
			for (unsigned int i = 0; i < r_poolSize; ++i)
			{
				bool valid;


				IndexedMesh& mesh = m_mesh.GetObjectWithStaticID(m_batches[i].mesh, valid);
				Material& material = m_material.GetObjectWithStaticID(m_batches[i].material, valid);

				TextureBinder::GetInstance().BindTexture(5, material.GetTexture());

				mesh.DrawBegin();
				{
					unsigned int r_numBatchItems = m_batches[i].transform.size();

					for (unsigned int j = 0; j < r_numBatchItems; ++j)
					{
						//
						Component::CTransform& trans = m_batches[i].transform[j];

						m_geometryPass.SetModel(trans.GetModel());
						m_geometryPass.SetMVP(trans.GetMVP(m_camera->GetViewProject()));

						mesh.DrawGroup();
					}

				}
				mesh.DrawEnd();
			}
			m_batches.clear();
		}

		void RenderSystem::DSPointLightStencilPass(unsigned int i)
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

			glm::mat4 mvp = m_camera->GetViewProject() * m_transform.GetModel(); // expensive
			m_stencilPass.SetMVP(mvp);

			m_pointLightStencilVolume->Draw();

			/*POINT LIGHT PASS*/

			m_gBuffer.BindForLightPass();

			m_pointLightPass.Enable();
			m_pointLightPass.SetMatSpecularPower(100);
			m_pointLightPass.SetEyeWorldPos(m_camera->GetPosition());

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

			m_pointLightStencilVolume->Draw();

			glCullFace(GL_BACK);
			glDisable(GL_BLEND);
		}

		void RenderSystem::DSDirectionalLightPass()
		{
			// used for tweening
			static float j = 0;
			j += 0.001f;

			m_gBuffer.BindForLightPass();
			m_dirLightPass.Enable();
			m_dirLightPass.SetMatSpecularPower(200);
			m_dirLightPass.SetEyeWorldPos(m_camera->GetPosition());

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);

			m_transform.Reset();

			glBindVertexArray(m_vaoQuad);

			/* animation code */
			dLight.direction = glm::vec3(0, cos(j * 5), sin(j * 5));
			////////////////////

			m_dirLightPass.SetDirectionalLight(dLight);
			m_dirLightPass.SetMVP(glm::mat4());

			// draw quad using the geometry shader
			glDrawArrays(GL_POINTS, 0, 1);

			glBindVertexArray(0);

			glDisable(GL_BLEND);
		}


		void RenderSystem::DSSkyboxPass()
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

			TextureBinder::GetInstance().BindTexture(7, m_skyboxTexture);

			m_skyboxPass.SetMVP(m_camera->GetViewProject() * m_transform.GetModel());

			m_skyboxCube->Draw();
			glDepthFunc(GL_LESS);

			glCullFace(OldCullFaceMode);
			glDepthFunc(OldDepthFuncMode);
		}

		void RenderSystem::DSFinalPass()
		{
			m_gBuffer.BindForFinalPass();

			static const GLuint width = Window::GetWidth();
			static const GLuint height = Window::GetHeight();

			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		}

	}
}