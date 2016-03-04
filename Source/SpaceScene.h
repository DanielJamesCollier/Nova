#pragma once

#include "Scene.h"
#include "CTransform.h"
#include "EntityManager.h"
#include "StaticPool.h"
#include "CTransform.h"
#define shipCount   100

namespace Nova
{
	namespace ScratchPad
	{
		class SpaceScene : public Scene
		{
		public:
			 SpaceScene();
			~SpaceScene();

			void Initialise() override;

			void Update(float delta)		 override;
			void Render(GeometryPass* gPass) override;

		private:
			ECS::Component::CTransform m_transform;
			float	  m_rot;

			ECS::EntityManager m_entityManager;
			StaticPool<ECS::Component::CTransform> m_pool;
		};
	}
}
