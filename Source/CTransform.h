#pragma once
#include <GLM\glm.hpp>

#include <GLM\gtx\transform.hpp>

namespace Nova
{
	namespace ECS
	{
		namespace Component
		{
			struct CTransform
			{

				CTransform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0))
					:
					m_position(pos),
					m_rotation(rot),
					m_scale(scale),
					m_model(glm::mat4()),
					m_updateModel(true)
				{

				}

				void Reset()
				{
					m_position = glm::vec3();
					m_rotation = glm::vec3();
					m_scale = glm::vec3(1, 1, 1);
				}

				const glm::mat4& GetModel()
				{
					if (!m_updateModel) return m_model;
					m_updateModel = false;

					glm::mat4 posMatrix = glm::translate(m_position);

					glm::mat4 rotX = glm::rotate(m_rotation.x, glm::vec3(1, 0, 0));
					glm::mat4 rotY = glm::rotate(m_rotation.y, glm::vec3(0, 1, 0));
					glm::mat4 rotZ = glm::rotate(m_rotation.z, glm::vec3(0, 0, 1));

					glm::mat4 mscale = glm::scale(m_scale);

					m_model = posMatrix * (rotZ * rotY * rotX) * mscale;

					return m_model;
				}

				glm::mat4 GetMVP(const glm::mat4& viewprojection)
				{
					return viewprojection * GetModel();
				}

				void SetPosition(const glm::vec3& position)
				{
					m_updateModel = true;
					m_position = position;
				}

				void SetRotation(const glm::vec3& rotation)
				{
					m_updateModel = true;
					m_rotation = rotation;
				}

				void SetScale(const glm::vec3& scale)
				{
					m_updateModel = true;
					m_scale = scale;
				}

			private:
				glm::vec3 m_position;
				glm::vec3 m_rotation;
				glm::vec3 m_scale;
				glm::mat4 m_model;
				bool      m_updateModel;
			};
		}	
	}
}


