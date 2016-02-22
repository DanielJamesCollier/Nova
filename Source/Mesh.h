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

	class Mesh
	{
	public:
		 Mesh(Vertex* vertices, GLuint vCount);
		 Mesh(const Mesh& mesh); // copy constructor
		 Mesh(const Mesh&& mesh); // move constructor
		~Mesh();

		void operator=(Mesh& mesh)
		{

		}

		void Draw();

	private:
	
		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_drawCount;
	};

	class IndexedMesh
	{
	public:
		 IndexedMesh(){}
		 IndexedMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		 IndexedMesh(Vertex* vertices, GLuint vCount, const std::vector<unsigned int> indices);
		~IndexedMesh();

		void DisposeGLResources();

		void Draw(DrawMode drawMode = DrawMode::NOVA_TRIANGLES) const;

		// when drawing multiple versions of the same object without instancing use this draw begin bind the vao and draw group draws the object
		// this saves on binding the vao multiple times
		void DrawBegin(DrawMode drawMode = DrawMode::NOVA_TRIANGLES) const;
		void DrawGroup() const;
		void DrawEnd() const;

		static unsigned int GetDrawCallCount();
		static void ResetDrawCalls();

	protected:

		GLuint m_vao;
		GLuint m_buffers[2]; // vbo and index buffer
		GLuint m_drawCount = 0;

	private:
		static unsigned int m_drawCallCount;
	};
}
