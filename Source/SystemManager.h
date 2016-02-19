#pragma once
#include "System.h"
#include <vector>
namespace Nova
{
	namespace ECS
	{
		class SystemManager
		{
		public:
			SystemManager();
			~SystemManager();

			void AddSystem(System& s);

			void Update();

		private:
			std::vector<System> m_systems;
		};
	}
}