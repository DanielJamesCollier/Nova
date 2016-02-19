#include "Box.h"


namespace Nova
{
	Box::Box(float width, float height)
	{
		SetSize(width, height);
	}
	
	Box::Box(const glm::vec2& size)
	{
		SetSize(size);
	}

	Box::~Box()
	{
	}

	const glm::vec2& Box::GetSize()
	{
		return m_size;
	}

	float Box::GetWidth() 
	{
		return m_size.x;
	}

	float Box::GetHeight()
	{
		return m_size.y;
	}

	void Box::SetSize(float width, float height)
	{
		m_size.x = width;
		m_size.y = height;
	}

	void Box::SetSize(const glm::vec2& size)
	{
		m_size = size;
	}

	void Box::SetWidth(float width)
	{
		m_size.x = width;
	}

	void Box::SetHeight(float height)
	{
		m_size.y = height;
	}

	void Box::ConstructBox()
	{
		//TODO 
	}
}
