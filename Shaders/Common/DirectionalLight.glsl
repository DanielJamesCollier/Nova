#IncludeGuard "Shaders/Common/light_structs.glsl"
#IncludeGuard "Shaders/Common/CalcDirectionLight.glsl"

const int                        MAX_DIRECTIONAL_LIGHTS = 50;
uniform DirectionalLight dLights[MAX_DIRECTIONAL_LIGHTS];
uniform int              numDirLights = 0;