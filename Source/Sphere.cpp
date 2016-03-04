#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Nova
{
	Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors)
		:
		m_radius(radius),
		m_rings(rings),
		m_sectors(sectors)
	{
		// create vertex array
		glCreateVertexArrays(1, &m_vao);

		//create vertex buffer and index buffer
		glCreateBuffers(2, m_buffers);

		std::vector<glm::vec3>    vertices;
		std::vector<unsigned int> indices;

		GenSphere(vertices, indices);

		glNamedBufferStorage(m_buffers[0], vertices.size() * sizeof(vertices[0]), &vertices[0], 0);
		glNamedBufferStorage(m_buffers[1], indices.size() * sizeof(indices[0]), &indices[0], 0);

		GLuint bindingIndex = 0;
		GLuint posLocation = 0;

		glVertexArrayAttribFormat(m_vao, posLocation, 3, GL_FLOAT, GL_FALSE, 0);

		glVertexArrayVertexBuffer(m_vao, bindingIndex, m_buffers[0], 0, sizeof(vertices[0]));
		glVertexArrayElementBuffer(m_vao, m_buffers[1]); // element aray buffer for indices

		glVertexArrayAttribBinding(m_vao, posLocation, bindingIndex);

		glEnableVertexArrayAttrib(m_vao, posLocation);

		m_indicesCount = indices.size();

	}

	Sphere::~Sphere()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(2, m_buffers);
	}

	void Sphere::GenSphere(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
	{
		float const R = 1.0f / static_cast<float>(m_rings   - 1);
		float const S = 1.0f / static_cast<float>(m_sectors - 1);

		for (unsigned int r = 0; r < m_rings; ++r) {
			for (unsigned int s = 0; s < m_sectors; ++s) {
				float const y = sin(-M_PI_2 + M_PI * r * R);
				float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
				float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);


				vertices.push_back(glm::vec3(x, y, z) * m_radius);
				AddIndices(indices, m_sectors, r, s);
			}
		}

	}

	void Sphere::AddIndices(std::vector<unsigned int>& indices,unsigned int sectors, unsigned int r, unsigned int s)
	{
		int curRow = r * m_sectors;
		int nextRow = (r + 1) * m_sectors;

		indices.push_back(curRow + s);
		indices.push_back(nextRow + s);
		indices.push_back(nextRow + (s + 1));

		indices.push_back(curRow + s);
		indices.push_back(nextRow + (s + 1));
		indices.push_back(curRow + (s + 1));
	}

}
