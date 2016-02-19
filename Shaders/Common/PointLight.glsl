#IncludeGuard "Shaders/Common/light_structs.glsl"
#IncludeGuard "Shaders/Common/CalcPointLight.glsl"

const int                  MAX_POINT_LIGHTS = 50;
uniform PointLight pLights[MAX_POINT_LIGHTS];
uniform int        numPointLights = 0;