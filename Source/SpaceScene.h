#pragma once

#include "Scene.h"
#include "Transform.h"
#include "EntityManager.h"
#define shipCount   200

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
		};


	}
}
