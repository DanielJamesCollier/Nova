#pragma once
#include <GLM\glm.hpp>

#include <GLM\gtx\transform.hpp>

namespace Nova
{
	namespace ECS
	{
		struct CTransform
		{

			CTransform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0))
				:
				position(pos),
				rotation(rot),
				scale(scale)
			{

			}

			void Reset()
			{
				position = glm::vec3();
				rotation = glm::vec3();
				scale = glm::vec3(1, 1, 1);
			}

			glm::mat4 GetModel() const
			{
				glm::mat4 posMatrix = glm::translate(position);

				glm::mat4 rotX = glm::rotate(rotation.x, glm::vec3(1, 0, 0));
				glm::mat4 rotY = glm::rotate(rotation.y, glm::vec3(0, 1, 0));
				glm::mat4 rotZ = glm::rotate(rotation.z, glm::vec3(0, 0, 1));

				glm::mat4 mscale = glm::scale(scale);

				return posMatrix * (rotZ * rotY * rotX) * mscale;
			}

			glm::mat4 GetMVP(const glm::mat4& viewprojection) const
			{
				return viewprojection * GetModel();
			}

			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
		};
	}
}


