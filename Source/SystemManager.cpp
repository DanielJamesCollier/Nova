#include "SystemManager.h"

namespace Nova
{
	namespace ECS
	{
		SystemManager::SystemManager()
		{

		}	

		SystemManager::~SystemManager()
		{

		}

		void SystemManager::AddSystem(System& system)
		{
			m_systems.push_back(system);
		}

		void SystemManager::Update()
		{
			for (System& s : m_systems)
			{
				s.Update();
			}
		}

	}
}