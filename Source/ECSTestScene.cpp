#include "ECSTestScene.h"

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
			SetSkyboxCubeMap(ResourceManager::GetCubeMap("spacebox")); // if this line isnt here the program crasshes, add a default cube map

			ECS::CRenderableTest model;
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

			unsigned int width = 50;
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