#pragma once
#include "CPointLight.h"
#include <GLM\glm.hpp>
namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			namespace Light
			{
				struct CSpotLight
				{
					CSpotLight(const CPointLight& base = CPointLight(), const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), float cutoff = 0.984800)
					{
						this->base = base;
						this->direction = direction;
						this->cutoff = cutoff;
					}

					CPointLight base;
					glm::vec3   direction;
					float	    cutoff;
				};
			}
		}
	}
}