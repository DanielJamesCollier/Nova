#version 450 core

#IncludeGuard "Shaders/Common/GammaCorrection.glsl"
#IncludeGuard "Shaders/Common/DirectionalLightDeferred.glsl"
#IncludeGuard "Shaders/Deferred/LightingPass/Common/GBuffer.glsl"
#IncludeGuard "Shaders/Deferred/LightingPass/Common/CalcTexCoord.glsl"
#IncludeGuard "Shaders/Deferred/LightingPass/Common/ReconstructNormalZ.glsl"

// shader ins and outputs
out vec4 finalPixel;

void main()
{
	// extract data from g buffer
	vec2 texCoord   = CalcTexCoord();
	vec3 position   = texture(gBufferPosition, texCoord).rgb;
	vec4 albedoSpec = texture(gBufferDiffuse,  texCoord).rgba;
	vec3 normal     = texture(gBufferNormal,   texCoord).rgb;
	//vec3 normal     = ReconstructNormalZ(texture(gBufferNormal,   texCoord).rg);
	
	finalPixel = vec4(albedoSpec.xyz, 1.0) * CalcDirectionLight(dLight, normal, position, albedoSpec.w);
	//finalPixel = GammaCorrection(finalPixel);
}