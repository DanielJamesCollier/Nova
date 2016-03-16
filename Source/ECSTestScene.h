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
			 ECSTestScene();
			~ECSTestScene();

			void Initialise() override;


			void Update(float delta) override;
			void Render() override;		

		private:
			void AddRenderableObjectToScene(unsigned int objectId, const ECS::Component::CTransform& transform, const ECS::Component::CRenderable& renderable);
		};
	}
}