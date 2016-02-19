#IncludeGuard "Shaders/Common/Material.glsl"

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal, vec3 worldPos, float specularIntensity)
{
	vec4 ambientColour  = vec4(light.colour,1.0f) * light.ambientIntensity;
	float diffuseFactor = dot(normal, -lightDirection);
	
	vec4 diffuseColour  = vec4(0,0,0,0);
	vec4 specularColour = vec4(0,0,0,0);
	
	if(diffuseFactor > 0)
	{
		diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

		//specular
		vec3 vertexToEye     = normalize(eyeWorldPos - worldPos);
		vec3 lightReflect    = normalize(reflect(lightDirection,normal));
		float specularFactor = dot(vertexToEye,lightReflect);
		if(specularFactor > 0)
		{
			specularFactor = pow(specularFactor, material.matSpecularPower);
			specularColour = vec4(light.colour * specularIntensity * specularFactor,1.0f);
		}
	}
	
	return (ambientColour + diffuseColour + specularColour);
}