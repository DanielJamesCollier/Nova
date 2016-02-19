#pragma once
#include "EntityManager.h"

namespace Nova
{
	namespace ECS
	{
		class System
		{
		public:
			System(EntityManager& em)
				:
				m_entityManager(em)
			{
			}

			~System()
			{
			}

			virtual void Update() = 0;
			

		private:
			EntityManager& m_entityManager;
		};


	}
}
