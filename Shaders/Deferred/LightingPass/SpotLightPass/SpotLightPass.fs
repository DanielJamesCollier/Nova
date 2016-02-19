#version 450 core

#IncludeGuard "Shaders/Common/GammaCorrection.glsl"
#IncludeGuard "Shaders/Common/SpotLightDeferred.glsl"
#IncludeGuard "Shaders/Deferred/LightingPass/Common/GBuffer.glsl"
#IncludeGuard "Shaders/Deferred/LightingPass/Common/CalcTexCoord.glsl"

// shader ins and outputs
out vec4 finalPixel;

void main()
{
	// extract data from g buffer
	vec2 texCoord   = CalcTexCoord();
	vec3 position   = texture(gBufferPosition, texCoord).rgb;
	vec4 albedoSpec = texture(gBufferDiffuse,  texCoord).rgba;
	vec3 normal     = texture(gBufferNormal,   texCoord).rgb;

	normal = normalize(normal);

	finalPixel = vec4(albedoSpec.xyz, 1.0) * CalcSpotLight(sLight, normal, position, albedoSpec.w);
	//finalPixel = GammaCorrection(finalPixel);
}