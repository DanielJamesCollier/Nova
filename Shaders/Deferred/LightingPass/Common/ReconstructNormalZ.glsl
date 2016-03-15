vec3 ReconstructNormalZ(vec2 normalXY)
{
	vec3 normalXYZ;
	normalXYZ.xy = normalXY;
	normalXYZ.z  = sqrt(1-dot(normalXYZ.xy,normalXYZ.xy));
	return normalXYZ;
}

vec3 ReconstructNormalZTest(vec2 normalXY)
{
	vec2 normalScaled = normalXY* 2.0 - 1.0;
	vec3 normalXYZ;
	normalXYZ.xy = normalScaled;
	normalXYZ.z  = sqrt(1-dot(normalXYZ.xy,normalXYZ.xy));

	normalize(normalXYZ);
	return normalXYZ;
}


