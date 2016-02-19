vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal, vec3 worldPos)
{
	vec4 ambientColour  = vec4(light.colour,1.0f) * light.ambientIntensity;
	float diffuseFactor = dot(normal, -lightDirection);
	
	vec4 diffuseColour  = vec4(0,0,0,0);
	vec4 specularColour = vec4(0,0,0,0);
	
	if(diffuseFactor > 0)
	{
		diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

		//specular
		vec3 vertexToEye = normalize(eyeWorldPos - fsWorldPos);
		vec3 lightReflect = normalize(reflect(lightDirection,normal));
		float specularFactor = dot(vertexToEye,lightReflect);
		if(specularFactor > 0)
		{
			specularFactor = pow(specularFactor, material.matSpecularPower);
			specularColour = vec4(light.colour * material.matSpecularIntensity * specularFactor,1.0f);
		}
	}
	
	return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcPointLight(struct PointLight p, vec3 normal, vec3 worldPos)
{
	vec3  lightDir  = worldPos - p.position;
	float distance = length(lightDir);
	lightDir       = normalize(lightDir);
	
	vec4 Colour = CalcLightInternal(p.base, lightDir, normal, worldPos);
	
	float Attenuation = p.atten.constant + 	p.atten.linear   * distance +
						p.atten.exp      * distance * distance;
						
	return Colour / Attenuation;	
}

vec4 CalcSpotLight(struct SpotLight s, vec3 Normal, vec3 worldPos)
{
     vec3 LightToPixel = normalize(worldPos - s.base.position);
     float SpotFactor = dot(LightToPixel, s.direction);

     if (SpotFactor > s.cutoff) {
         vec4 Color = CalcPointLight(s.base, Normal, worldPos);
         return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - s.cutoff));
     }
     else {
         return vec4(0,0,0,0);
     }
}

vec4 CalcDirectionLight(struct DirectionalLight d, vec3 normal, vec3 worldPos)
{
	return CalcLightInternal(d.base, d.direction, normal, worldPos);
}