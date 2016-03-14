#include "Mesh.h"
#include <vector>
#include <iostream>

namespace Nova
{

	IndexedMesh::IndexedMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
		: 
		m_indicesCount(indices.size())
	{
		//OpenGL 4.5 / ARB_direct_state_access

		// create vertex array object
		glCreateVertexArrays(1, &m_vao);

		// create buffer and add data
		glCreateBuffers(2, m_buffers);
		glNamedBufferStorage(m_buffers[0], vertices.size() * sizeof(vertices[0]), &vertices[0], 0);
		glNamedBufferStorage(m_buffers[1], indices.size() * sizeof(indices[0]), &indices[0], 0);

		GLuint bindingIndex = 0;

		GLuint posLocation  = 0;
		GLuint texLocation  = 1;
		GLuint normLocation = 2;
		GLuint tangLocation = 3;

		// binding index is used because multiple vbos can be used in a vao


		// Setup the formats
		glVertexArrayAttribFormat(m_vao, posLocation, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos)); // 0
		glVertexArrayAttribFormat(m_vao, texLocation, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex)); // 28
		glVertexArrayAttribFormat(m_vao, normLocation, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, norm));
		glVertexArrayAttribFormat(m_vao, tangLocation, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tang));

		// THIS ONE SPECIFIES WHICH BUFFER TO USE FOR THE CHOSEN BINDING INDEX, AN OFFSET INTO THE BUFFER, AND THE STRIDE OF THE VERTEX FORMAT
		glVertexArrayVertexBuffer(m_vao, bindingIndex, m_buffers[0], 0, sizeof(vertices[0]));
		glVertexArrayElementBuffer(m_vao, m_buffers[1]); // element aray buffer for indices

		// Link
		// THIS ONE CONNECTS ATTRIB LOCATIONS TO THE BINDING INDEX
		glVertexArrayAttribBinding(m_vao, posLocation, bindingIndex);
		glVertexArrayAttribBinding(m_vao, texLocation, bindingIndex);
		glVertexArrayAttribBinding(m_vao, normLocation, bindingIndex);
		glVertexArrayAttribBinding(m_vao, tangLocation, bindingIndex);

		// Enable
		glEnableVertexArrayAttrib(m_vao, posLocation);
		glEnableVertexArrayAttrib(m_vao, texLocation);
		glEnableVertexArrayAttrib(m_vao, normLocation);
		glEnableVertexArrayAttrib(m_vao, tangLocation);
	}


	IndexedMesh::~IndexedMesh()
	{
		std::cout << "mesh destructed" << std::endl;
	}

	void IndexedMesh::DisposeGLResources() const
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(2, m_buffers);
	}

	void IndexedMesh::Draw(DrawMode drawMode) const
	{
		glBindVertexArray(m_vao);

		if (drawMode == DrawMode::NOVA_TRIANGLES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
		}
		else if (drawMode == DrawMode::NOVA_WIREFRAME)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
			
		}

		glBindVertexArray(0);
	}

	void IndexedMesh::DrawBegin(DrawMode drawMode) const
	{
		glBindVertexArray(m_vao);

		if (drawMode == DrawMode::NOVA_TRIANGLES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (drawMode == DrawMode::NOVA_WIREFRAME)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	void IndexedMesh::DrawGroup() const
	{
		glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
	}

	void IndexedMesh::DrawEnd() const
	{
		glBindVertexArray(0);
	}
}
