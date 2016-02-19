#pragma once
#include "Mesh.h"
#include <GLEW\glew.h>
#include <vector>

namespace Nova
{
	class Sphere : public IndexedMesh
	{
	public:
		Sphere(float radius, unsigned int rings, unsigned int sectors);
		~Sphere();

	private:
		float        m_radius;
		unsigned int m_rings;
		unsigned int m_sectors;

		void GenSphere(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);
		void AddIndices(std::vector<unsigned int>& indices, unsigned int sectors, unsigned int r, unsigned int s);
		
	};
}
