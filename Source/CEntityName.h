#pragma once
#include <string>

namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			struct CEntityName
			{
				
				CEntityName(const std::string n)
					:
					name(n)
				{
				}

				std::string name;
			};
		}
	}
}