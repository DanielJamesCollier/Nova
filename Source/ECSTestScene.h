#pragma once
#include "Scene.h"
#include "RenderSystem.h"
#include "StaticPool.h"
#include "CRenderable.h"
#include "CTransform.h"

namespace Nova
{
	namespace ScratchPad
	{
		class ECSTestScene : public Scene
		{
		public:
			 ECSTestScene(GeometryPass& gPass);
			~ECSTestScene();

			void Initialise() override;

			void Update(float delta) override;
			void Render(GeometryPass* gPass) override;

		private:
			StaticPool<ECS::Component::CTransform>  m_transformComps;
			StaticPool<ECS::Component::CRenderable> m_renderComps;
			ECS::RenderSystem m_renderSystem;
		};
	}
}