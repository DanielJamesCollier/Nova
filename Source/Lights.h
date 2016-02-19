#pragma once

#include <GLM\glm.hpp>

namespace Nova
{
	struct BaseLight
	{
		BaseLight(const glm::vec3& colour = glm::vec3(1.0f,1.0f,1.0f), float ambientInensity = 0.1f, float diffuseIntensity = 0.7f)
		{
			this->colour = colour;
			this->ambientIntensity = ambientInensity;
			this->diffuseIntensity = diffuseIntensity;
		}

		glm::vec3 colour;
		float     ambientIntensity;
		float     diffuseIntensity;
	};

	struct DirectionalLight
	{
		DirectionalLight(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), const BaseLight& base = BaseLight())
		{
			this->direction = direction;
			this->base = base;
		}

		glm::vec3 direction;
		BaseLight base;

	};

	struct Attenuation
	{
		Attenuation(float constant = 1.0f, float linear = 0.001f, float exp = 10.0f)
		{
			this->constant = constant;
			this->linear = linear;
			this->exp = exp;
		}

		float constant;
		float linear;
		float exp;

	};

	struct PointLight
	{
		PointLight(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const Attenuation& attenuation = Attenuation(), const BaseLight& base = BaseLight())
		{
			this->position = position;
			this->atten = attenuation;
			this->base = base;
		}

		glm::vec3   position;
		Attenuation atten;
		BaseLight   base;
		
	};

	struct SpotLight
	{
		SpotLight(const PointLight& base = PointLight(), const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), float cutoff = 0.984800)
		{
			this->base = base;
			this->direction = direction;
			this->cutoff = cutoff;
		}
		
		PointLight base;
		glm::vec3  direction;
		float	   cutoff;
		
	};

	class Lights
	{
	public:
		
		static float CalcPointLightBSphere2(const PointLight& Light)
		{
			float MaxChannel = fmax(fmax(Light.base.colour.x, Light.base.colour.y), Light.base.colour.z);

			float ret = (-Light.atten.linear + sqrtf(Light.atten.linear * Light.atten.linear -
				4 * Light.atten.exp * (Light.atten.exp - 256 * MaxChannel * Light.base.diffuseIntensity)))
				/
				2 * Light.atten.exp;
			return ret;
		}

		static float CalcPointLightBSphere(const PointLight& Light)
		{
			float MaxChannel = fmax(fmax(Light.base.colour.x, Light.base.colour.y), Light.base.colour.z);

			float ret = (-Light.atten.linear + sqrtf(Light.atten.linear * Light.atten.linear - 4 * Light.atten.exp * (Light.atten.exp - 256 * MaxChannel * Light.base.diffuseIntensity)))
				/
				2 * Light.atten.exp;

			return ret;
		}
	};
	
}
