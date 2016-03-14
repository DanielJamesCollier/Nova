#pragma once
#include "DeferredPass.h"
#include <GLM\glm.hpp>

namespace Nova
{
	class GeometryPass : public DeferredPass
	{
	public:
		GeometryPass();
		~GeometryPass();

		bool Init();

		void SetMVP(const glm::mat4& mvp);
		void SetModel(const glm::mat4& model);

	private:
	
		GLuint m_MVP;
		GLuint m_Model;
	};

}
