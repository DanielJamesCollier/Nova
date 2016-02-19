#pragma once
#include "GLEW\glew.h"
#include "LightPass.h"
#include "Lights.h"

namespace Nova
{
	class DirectionalLightPass : public LightPass
	{
	public:
		DirectionalLightPass();
		~DirectionalLightPass();

		bool Init();

		void SetDirectionalLight(const DirectionalLight& dLight);

	private:

		struct {
			GLuint Color;
			GLuint AmbientIntensity;
			GLuint DiffuseIntensity;
			GLuint Direction;
		} m_dirLightLocation;

	};


}
