#IncludeGuard "Shaders/Common/CalcSpotLight.glsl"

const int                        MAX_SPOT_LIGHTS  = 50;
uniform SpotLight        sLights[MAX_SPOT_LIGHTS];
uniform int              numSpotLights = 0;