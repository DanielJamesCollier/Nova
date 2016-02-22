#include "SpaceScene.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "GeometryPass.h"
#include "MathUtil.h"
#include "Sphere.h"

namespace Nova
{
	namespace ScratchPad
	{
		SpaceScene::SpaceScene()
			:
			m_pool(100)
		{
					
		}

		void SpaceScene::Initialise()
		{
			if (GetInitialised()) return;

			SetInitialised(true);

			ResourceManager::CacheIndexedMesh("starwars", "Models/starwars/ARC170.3DS");
			ResourceManager::CacheIndexedMesh("spaceship", "Models/ship/Sample_Ship.3ds");
			ResourceManager::CacheIndexedMesh("moon", "Models/planets/moon/moon.obj");
			ResourceManager::CacheIndexedMesh("cube", "Models/Crate/Crate1.obj");
			ResourceManager::CacheIndexedMesh("F-16C", "Models/F-16C/F-16C_FightingFalcon.obj");

			ResourceManager::AddMaterial("crate", new Material("Models/Crate/crate_1.png", "Models/Crate/crate_1_NRM.png", "Models/Crate/crate_1_SPEC.png", 400));
			ResourceManager::AddMaterial("starwars", new Material("Models/starwars/Arc170_blinn1.png", "Models/starwars/Arc170_blinn1_NRM.png", "Models/starwars/Arc170_blinn1_SPEC.png", 10));
			ResourceManager::AddMaterial("spaceship", new Material("Models/ship/sh3.png", "Models/ship/sh3_NRM.png", "Models/ship/sh3_NRM_SPEC.png", 20));
			ResourceManager::AddMaterial("moon", new Material("Models/planets/moon/MoonMap2_2500x1250.png", "Models/planets/moon/MoonMap2_2500x1250_NRM.png", "Models/planets/moon/MoonMap2_2500x1250_NRM.png", 10));
			ResourceManager::AddMaterial("F16C", new Material("Models/F-16C/F-16C_FightingFalcon_P01.png", "Models/F-16C/F-16C_FightingFalcon_N.png", "Models/F-16C/F-16C_FightingFalcon_P01_SPEC.png", 40));

			////// cubemap //////
			std::string space = "Textures/Skyboxes/spacebox/";

			ResourceManager::CacheCubeMap("spacebox",
				space + "space_right1.png",
				space + "space_left2.png",
				space + "space_top3.png", // top of sphere
				space + "space_bottom4.png", // bottom of sphere
				space + "space_front5.png",
				space + "space_back6.png");
			////////////////

			SetSkyboxCubeMap(ResourceManager::GetCubeMap("spacebox"));

			m_pool.AddObject(0,ECS::CTransform());
		}

		SpaceScene::~SpaceScene()
		{
			// TODO remove all items from cache
		
		}

		void SpaceScene::Update(float delta)
		{
			UpdateActiveCamera(delta);
		}

		void SpaceScene::Render(GeometryPass* gPass)
		{

			 /* POSSIBLE ERROR WITH DRAW GROUP */


			/////////* spaceship */////
			m_transform.Reset();
			static const IndexedMesh* starwars = ResourceManager::GetIndexedMesh("starwars");

			ResourceManager::GetMaterial("starwars")->Bind(0);

			starwars->DrawBegin();
			m_transform.GetScale() = glm::vec3(0.001f);
			for (int i = 0; i < 50; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					m_transform.GetPos() = glm::vec3(i * 5, 0, j * 5);
					gPass->SetMVP(m_transform.GetMVP(GetActiveCamera().GetViewProject()));
					gPass->SetModel(m_transform.GetModel());
					starwars->DrawGroup();
				}
			}
			starwars->DrawEnd();
			///////////////////////////


			///// space ship /////
			m_transform.Reset();

			ResourceManager::GetMaterial("spaceship")->Bind(0);

			static const IndexedMesh* ship = ResourceManager::GetIndexedMesh("spaceship");

			m_transform.GetRot() = glm::vec3(-90, -90, 0);
			m_transform.GetScale() = glm::vec3(10, 10, 10);

			gPass->SetMVP(m_transform.GetMVP(GetActiveCamera().GetViewProject()));
			gPass->SetModel(m_transform.GetModel());
			ship->Draw();
			//////////////////////


			/* MOON */
			///////////////////////////////////
			ResourceManager::GetMaterial("moon")->Bind(0);
			static const IndexedMesh* moon = ResourceManager::GetIndexedMesh("moon");


			m_transform.Reset();

			m_transform.GetScale() = glm::vec3(0.1, 0.1, 0.1);
			m_transform.GetPos() = glm::vec3(25, 10, 25);
			m_transform.GetRot() = glm::vec3(0, m_rot * 5, 10);

			gPass->SetMVP(m_transform.GetMVP(GetActiveCamera().GetViewProject()));
			gPass->SetModel(m_transform.GetModel());

			moon->Draw();

			m_transform.Reset();

			m_transform.GetPos() = glm::vec3(200, 0, 25);
			m_transform.GetRot() = glm::vec3(0, m_rot * 5, 0);

			gPass->SetMVP(m_transform.GetMVP(GetActiveCamera().GetViewProject()));
			gPass->SetModel(m_transform.GetModel());

			moon->Draw();
		///////////////////////////


		//////////// cube ////////////
		ResourceManager::GetMaterial("crate")->Bind(0);
		m_transform.Reset();



		gPass->SetMVP(m_transform.GetMVP(GetActiveCamera().GetViewProject()));
		gPass->SetModel(m_transform.GetModel());
		ResourceManager::GetIndexedMesh("cube")->Draw();
		////////////////////////////


		//////// F-16C ///////
		//m_transform.Reset();

		//m_transform.GetRot() = glm::vec3(MathUtil::DegreesToRadiansf(-90), 0, 0);




		//gPass->SetMVP(m_transform.GetMVP(GetActiveCamera().GetViewProject()));
		//gPass->SetModel(m_transform.GetModel());
		//ResourceManager::GetMaterial("F16C")->Bind(0);
		//ResourceManager::GetIndexedMesh("F-16C")->Draw();
		//////////////////////


		//////////////
		}

	}
}
