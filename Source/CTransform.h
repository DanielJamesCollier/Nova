#pragma once
#include <GLM\glm.hpp>
namespace Nova
{
	namespace ECS
	{
		struct CTransform
		{
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
		};
	}
}


