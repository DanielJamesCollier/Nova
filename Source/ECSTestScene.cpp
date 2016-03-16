#include "ECSTestScene.h"
#include "Plane.h"
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
		ECSTestScene::ECSTestScene()
			:
			Scene(MAX_ENTITES,MAX_ENTITES)
		{
		
		}

		ECSTestScene::~ECSTestScene()
		{

		}

		void ECSTestScene::Initialise()
		{
			if (GetInitialised()) return;
			SetInitialised(true);
			m_renderSystem.SetCamera(&GetActiveCamera());


			// resources //
			/////////////////////////////
			// meshes
				unsigned int ID_MoonMesh  = m_renderSystem.AddMesh("Models/planets/moon/moon.obj");
				unsigned int ID_spaceship = m_renderSystem.AddMesh("Models/starwars/ARC170.3DS");
				unsigned int ID_checkerPlaneMesh = m_renderSystem.AddMesh("Models/shapes/checkerPlane.obj");
			
			//materials
				unsigned int ID_moonMat      = m_renderSystem.AddMaterial(Material("Models/planets/moon/MoonMap2_2500x1250.png", "Models/planets/moon/MoonMap2_2500x1250_NRM.png", "Models/planets/moon/MoonMap2_2500x1250_NRM.png", 10));
				unsigned int ID_spaceshipMat = m_renderSystem.AddMaterial(Material("Models/starwars/Arc170_blinn1.png", "Models/starwars/Arc170_blinn1_NRM.png", "Models/starwars/Arc170_blinn1_SPEC.png", 10));
				unsigned int ID_checkerPlaneMat = m_renderSystem.AddMaterial(Material("Models/shapes/checker.png", "Models/shapes/checker_NRM.png", "Models/shapes/checker_SPEC.png",10));
			////////////////////////////


			////// cubemap //////
			std::string redsky = "Textures/Skyboxes/redsky/";

			ResourceManager::CacheCubeMap("redsky",
				redsky + "r.png",
				redsky + "l.png",
				redsky + "up.png", // top of sphere
				redsky + "bot.png", // bottom of sphere
				redsky + "front.png",
				redsky + "back.png");

			SetSkyboxCubeMap(ResourceManager::GetCubeMap("redsky"));
			////////////////

			

			/* star wars space ship*/
			/////////////////
			{
				ECS::Component::CRenderable model;
				ECS::Component::CTransform  trans;


				model.material = ID_spaceshipMat;
				model.mesh     = ID_spaceship;


				trans.SetScale(glm::vec3(0.0001, 0.0001, 0.0001));

				unsigned int width = 20;

				for (unsigned int i = 0; i < width; ++i)
				{
					for (unsigned int j = 0; j < width; ++j)
					{
						trans.SetPosition(glm::vec3(j, rand() % 30, i));
						trans.SetRotation(glm::vec3(rand() % 20, rand() % 20, rand() % 20));

						AddRenderableObjectToScene(i * width + j, trans, model);
					}
				}
			}
		
			///////////////////
		
	

			/* MOON */
			///////////////////
			{
				ECS::Component::CRenderable model;
				ECS::Component::CTransform  trans;

				unsigned int moonID = 1000;

				// set transform component vars
				trans.SetPosition(glm::vec3(0, 0, -5));
				trans.SetScale(glm::vec3(0.1, 0.1, 0.1));

				// set model component vars
				model.mesh     = ID_MoonMesh;
				model.material = ID_moonMat;

				// add components
				AddRenderableObjectToScene(moonID, trans, model);
			}
			///////////////////


			/*PLANE*/
			{
				ECS::Component::CRenderable model;
				ECS::Component::CTransform  trans;

				model.material = ID_checkerPlaneMat;
				model.mesh     = ID_checkerPlaneMesh;

				trans.SetPosition(glm::vec3(-30,0,0));
				trans.SetScale(glm::vec3(30, 30, 30));

				AddRenderableObjectToScene(1010, trans, model);
			}
			
		}

		void ECSTestScene::AddRenderableObjectToScene(unsigned int objectId, const ECS::Component::CTransform& transform, const ECS::Component::CRenderable& renderable)
		{
			m_transformComps.AddObject(objectId, transform);
			m_renderComps.AddObject(objectId,renderable);		
		}

		void ECSTestScene::Update(float delta)
		{
			UpdateActiveCamera(delta);
		}

		void ECSTestScene::Render()
		{
			m_renderSystem.Update();
		}
	}
}