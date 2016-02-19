#IncludeGuard "Shaders/Common/CalcLightInternal.glsl"

vec4 CalcPointLight(struct PointLight p, vec3 normal, vec3 worldPos, float specularIntensity)
{
	vec3  lightDir  = worldPos - p.position;
	float distance = length(lightDir);
	lightDir       = normalize(lightDir);
	
	vec4 Colour = CalcLightInternal(p.base, lightDir, normal, worldPos, specularIntensity);
	
	float Attenuation = p.atten.constant + 	p.atten.linear   * distance +
						p.atten.exp      * distance * distance;
						
	return Colour / Attenuation;	
}