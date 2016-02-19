#pragma once
#include <GLM\glm.hpp>
#include "Mesh.h"

namespace Nova
{
	class Box : public IndexedMesh
	{
	public:
				 Box(float width, float height);
		explicit Box(const glm::vec2& size);
		~Box();

		const glm::vec2& GetSize();
		float GetWidth();
		float GetHeight();
		
		void SetSize(float width, float height);
		void SetSize(const glm::vec2& size);
		void SetWidth(float width);
		void SetHeight(float height);

	private:
		glm::vec2 m_size;

		void ConstructBox();
	};


}
