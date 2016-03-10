#pragma once
#include "CAmbientLight.h"
#include "CAttenuation.h"
#include <GLM\glm.hpp>

namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			namespace Light
			{
				struct CPointLight
				{
					CPointLight(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const CAttenuation& attenuation = CAttenuation(), const CAmbientLight& base = CAmbientLight())
					{
						this->position = position;
						this->atten = attenuation;
						this->base = base;
					}

					glm::vec3     position;
					CAttenuation  atten;
					CAmbientLight base;
				};
			}
		}
	}
}