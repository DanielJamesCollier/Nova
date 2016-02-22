#include "ECSTestScene.h"

namespace Nova
{
	#define MAX_ENTITES 1000
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
		
			if (model.mesh = m_renderSystem.AddMesh("Models/starwars/ARC170.3DS") == -1)
			{
				std::cout << "the model was not loaded" << std::endl;
			}

			model.material = ResourceManager::GetMaterial("starwars");
			trans.scale = glm::vec3(0.001, 0.001, 0.001);

			for (unsigned int i = 0; i < 500; ++i)
			{
				
					m_renderComps.AddObject(i , model);

					trans.position = glm::vec3(0,0, i);
					m_transformComps.AddObject(i , trans);
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