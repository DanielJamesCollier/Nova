#IncludeGuard "Shaders/Common/CalcLightInternal.glsl"

vec4 CalcDirectionLight(struct DirectionalLight dLight, vec3 normal, vec3 worldPos,  float specularIntensity)
{
	return CalcLightInternal(dLight.base, dLight.direction, normal, worldPos, specularIntensity);
}