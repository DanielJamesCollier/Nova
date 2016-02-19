#include "Transform.h"
#include <GLM\gtx\transform.hpp>


namespace Nova
{
	Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
		:
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{
	}


	Transform::~Transform()
	{
	}

	void Transform::Reset()
	{
		this->m_pos   = glm::vec3();
		this->m_rot   = glm::vec3();
		this->m_scale = glm::vec3(1, 1, 1);
	}

	glm::mat4 Transform::GetModel() const
	{
		glm::mat4 posMatrix = glm::translate(m_pos);

		glm::mat4 rotX = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotY = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZ = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));

		glm::mat4 scale = glm::scale(m_scale);

		return posMatrix * (rotZ * rotY * rotX) * scale;
	}

	glm::mat4 Transform::GetMVP(const glm::mat4& viewprojection) const
	{
		return viewprojection * GetModel();
	}

}
