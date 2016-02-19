#pragma once
#include "LightPass.h"
#include "Lights.h"

namespace Nova
{
	class SpotLightPass : public LightPass
	{
	public:
		SpotLightPass();
		~SpotLightPass();

		bool Init();

		void SetSpotLight(const SpotLight& sLight);

	private:

		struct 
		{
			GLuint Color;
			GLuint AmbientIntensity;
			GLuint DiffuseIntensity;
			GLuint Position;
			GLuint Direction;
			GLuint Cutoff;
			struct 
			{ // attenuation
				GLuint Constant;
				GLuint Linear;
				GLuint Exp;
			} Atten;
		} m_spotLightLocation;

	};


}
