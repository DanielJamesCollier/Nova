vec3 ReconstructNormalZ(vec2 normalXY)
{
	vec3 normalXYZ;
	normalXYZ.xy = normalXY;
	normalXYZ.z  = sqrt(1-dot(normalXYZ.xy,normalXYZ.xy));
	return normalXYZ;
}

