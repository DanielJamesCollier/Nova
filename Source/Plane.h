#pragma once
#include <GLM\glm.hpp>
#include "Mesh.h"

namespace Nova
{
	class Plane : public IndexedMesh
	{
	private:
		GLuint m_width, m_height;
	public:
		Plane(GLuint width, GLuint height);

	};
}