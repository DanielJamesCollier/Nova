#include "ECSTestScene.h"

namespace Nova
{
	namespace ScratchPad
	{
		ECSTestScene::ECSTestScene(GeometryPass& gPass)
			:
			m_renderComps(100),
			m_transformComps(100),
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

			model.mesh = ResourceManager::GetIndexedMesh("starwars");
			model.material = ResourceManager::GetMaterial("starwars");
			m_renderComps.AddObject(0, model);

			ECS::CTransform trans;
			trans.scale = glm::vec3(0.001, 0.001, 0.001);
			m_transformComps.AddObject(0, trans);

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