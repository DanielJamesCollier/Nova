vec4 GammaCorrection(vec4 pixel)
{
	float gamma = 2.2;
	return vec4(pow(pixel.xyz, vec3(1.0 / gamma)), 1.0);
}