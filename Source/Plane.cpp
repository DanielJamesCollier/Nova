#pragma once
#include "Plane.h"

namespace Nova
{
	Plane::Plane(GLuint width, GLuint height)
		:
		m_width(width),
		m_height(height)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		Vertex v1;
		Vertex v2;
		Vertex v3;
		Vertex v4;

		GLuint w = width / 2;

		// positions
		v1.pos = glm::vec3(-1.0 * w, 0.0,  1.0 * w);
		v2.pos = glm::vec3( 1.0 * w, 0.0,  1.0 * w);
		v3.pos = glm::vec3(-1.0 * w, 0.0, -1.0 * w);
		v3.pos = glm::vec3( 1.0 * w, 0.0, -1.0 * w);

		v1.tex = glm::vec2(0, 0);
		v2.tex = glm::vec2(1, 0);
		v3.tex = glm::vec2(1, 1);
		v4.tex = glm::vec2(0, 1);

		v1.norm = glm::vec3(0, 1, 0);
		v2.norm = glm::vec3(0, 1, 0);
		v3.norm = glm::vec3(0, 1, 0);
		v4.norm = glm::vec3(0, 1, 0);


		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);

		indices.push_back(2);
		indices.push_back(4);
		indices.push_back(3);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		CreateIndexedMesh(vertices, indices);
	}
}