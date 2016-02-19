#IncludeGuard "Shaders/Common/CalcPointLight.glsl"

vec4 CalcSpotLight(struct SpotLight s, vec3 Normal, vec3 worldPos, float specularIntensity)
{
     vec3 LightToPixel = normalize(worldPos - s.base.position);
     float SpotFactor = dot(LightToPixel, s.direction);

     if (SpotFactor > s.cutoff) 
	 {
         vec4 Color = CalcPointLight(s.base, Normal, worldPos,specularIntensity);
         return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - s.cutoff));
     }
     else
	 {
         return vec4(0,0,0,0);
     }
}