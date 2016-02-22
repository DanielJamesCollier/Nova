#pragma once
#include "EntityManager.h"

namespace Nova
{
	namespace ECS
	{
		class System
		{
		public:
			System()
			{
			}

			virtual ~System()
			{
			}

			virtual void Update() = 0;
			

		private:
		
		};


	}
}
