#pragma once
#include "LightPass.h"
#include "Lights.h"

namespace Nova
{
	class PointLightPass : public LightPass
	{
	public:
		PointLightPass();
		~PointLightPass();

		bool Init();

		void SetPointLight(const PointLight& pLight);

	private:

		struct {
			GLuint Color;
			GLuint AmbientIntensity;
			GLuint DiffuseIntensity;
			GLuint Position;
			struct {
				GLuint Constant;
				GLuint Linear;
				GLuint Exp;
			} Atten;
		} m_pointLightLocation;

	};


}

