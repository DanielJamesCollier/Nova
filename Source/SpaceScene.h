#pragma once

#include "Scene.h"
#include "CTransform.h"
#include "EntityManager.h"
#include "StaticPool.h"
#include "Transform.h"
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
			Transform m_transform;
			float	  m_rot;

			ECS::EntityManager m_entityManager;
			StaticPool<ECS::CTransform> m_pool;
		};
	}
}
