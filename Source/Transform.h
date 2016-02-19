#pragma once
#include <GLM\glm.hpp>

namespace Nova
{
	class Transform
	{
	public:
		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0));
		~Transform();

		void Reset();

		glm::mat4 GetModel() const;
		glm::mat4 GetMVP(const glm::mat4& viewprojection) const;

		inline glm::vec3& GetPos() { return m_pos; }
		inline glm::vec3& GetRot() { return m_rot; }
		inline glm::vec3& GetScale() { return m_scale; }


	private:
		glm::vec3 m_pos, m_rot, m_scale;
	};


}
