#pragma once
#include <GLM\glm.hpp>

namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			namespace Light
			{
				struct CAmbientLight
				{
					CAmbientLight(const glm::vec3& colour = glm::vec3(1.0f, 1.0f, 1.0f), float ambientInensity = 0.1f, float diffuseIntensity = 0.7f)
					{
						this->colour = colour;
						this->ambientIntensity = ambientInensity;
						this->diffuseIntensity = diffuseIntensity;
					}

					glm::vec3 colour;
					float     ambientIntensity;
					float     diffuseIntensity;
				};
			}
		}
	}
}
