#pragma once
#include "CAmbientLight.h"
#include <GLM\glm.hpp>

namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			namespace Light
			{
				struct CDirectionalLight
				{
					CDirectionalLight(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), const CAmbientLight& base = CAmbientLight())
					{
						this->direction = direction;
						this->base = base;
					}

					glm::vec3 direction;
					CAmbientLight base;
				};
			}
		}
	}
}
