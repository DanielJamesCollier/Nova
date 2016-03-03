#include "ECSTestScene.h"

/*
BUGS

- program crashes when initialise does not call the SetSkyboxCube() func
- the add material function of Render System does not account for failed material loading
- materials and meshes should abstract the opengl resource creation into a GLCore class
- look at a better way of handeling the camera for the render system
- add instancing 
- look at a better way to partition the active / dead entities - currently the render system loops over all

*/

namespace Nova
{
	#define MAX_ENTITES 2500
	namespace ScratchPad
	{
		ECSTestScene::ECSTestScene(GeometryPass& gPass)
			:
			m_renderComps(MAX_ENTITES),
			m_transformComps(MAX_ENTITES),
			m_renderSystem(gPass,m_transformComps,m_renderComps)
		{
		
		}

		ECSTestScene::~ECSTestScene()
		{

		}

		void ECSTestScene::Initialise()
		{
			if (GetInitialised()) return;
			SetInitialised(true);

			////// cubemap //////
			std::string redsky = "Textures/Skyboxes/redsky/";

			ResourceManager::CacheCubeMap("redsky",
				redsky + "r.png",
				redsky + "l.png",
				redsky + "up.png", // top of sphere
				redsky + "bot.png", // bottom of sphere
				redsky + "front.png",
				redsky + "back.png");
			////////////////

			SetSkyboxCubeMap(ResourceManager::GetCubeMap("redsky"));

			ECS::CRenderable model;
			ECS::CTransform      trans;

			int starwarsModelLocation;
		
			if (starwarsModelLocation = model.mesh = m_renderSystem.AddMesh("Models/starwars/ARC170.3DS") == -1)
			{
				std::cout << "the model was not loaded" << std::endl;
			}

			std::cout << "model location: " << starwarsModelLocation << std::endl;

			Material mat("Models/starwars/Arc170_blinn1.png", "Models/starwars/Arc170_blinn1_NRM.png", "Models/starwars/Arc170_blinn1_SPEC.png", 10);

			model.material = m_renderSystem.AddMaterial(mat);
			trans.scale = glm::vec3(0.0001, 0.0001, 0.0001);

			unsigned int width = 20;
			for (unsigned int i = 0; i < width; ++i)
			{
				for (unsigned int j = 0; j < width; ++j)
				{

			
					m_renderComps.AddObject(i * width + j, model);

					trans.position = glm::vec3(j , 0, i);
					m_transformComps.AddObject(i * width + j, trans);
				}
					
			}
		
			m_renderSystem.SetCamera(&GetActiveCamera());

			
			// moon
			{
				unsigned int moonID = 402;
				Material moon_mat("Models/planets/moon/MoonMap2_2500x1250.png", "Models/planets/moon/MoonMap2_2500x1250_NRM.png", "Models/planets/moon/MoonMap2_2500x1250_NRM.png", 10);

				// set transform component vars
				trans.Reset();
				trans.position = glm::vec3(0, 0, -5);
				trans.scale = glm::vec3(0.1, 0.1, 0.1);

				// set model component vars
				model.mesh = m_renderSystem.AddMesh("Models/planets/moon/moon.obj");
				model.material = m_renderSystem.AddMaterial(moon_mat);

				std::cout << "mesh index id: " << model.mesh << std::endl;

				// add components
				m_transformComps.AddObject(moonID, trans);
				m_renderComps.AddObject(moonID, model);


			}
			
			
		}

		void ECSTestScene::Update(float delta)
		{
			UpdateActiveCamera(delta);
		}

		void ECSTestScene::Render(GeometryPass* gPass)
		{
			m_renderSystem.Update();
		}
	}
}