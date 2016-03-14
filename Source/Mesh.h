#pragma once
#include "Vertex.h"
#include "GLEW\glew.h"
#include <GLM\glm.hpp>
#include <vector>

namespace Nova
{

	enum DrawMode
	{
		NOVA_TRIANGLES,
		NOVA_WIREFRAME,
		
	};

	class IndexedMesh
	{
	public:
		 IndexedMesh(){}
		 IndexedMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		~IndexedMesh();

		void DisposeGLResources() const;

		void Draw(DrawMode drawMode = DrawMode::NOVA_TRIANGLES) const;

		// when drawing multiple versions of the same object without instancing use this draw begin bind the vao and draw group draws the object
		// this saves on binding the vao multiple times
		void DrawBegin(DrawMode drawMode = DrawMode::NOVA_TRIANGLES) const;
		void DrawGroup() const;
		void DrawEnd() const;


	protected:

		GLuint m_vao;
		GLuint m_buffers[2]; // vbo and index buffer
		GLuint m_indicesCount = 0;
	};
}
