#IncludeGuard "Shaders/Deferred/LightingPass/Common/GBuffer.glsl"

uniform mat4 viewProjectionInverseMatrix;

float linearZ(float z)
{
  const float f = 1000.0;
  const float n = 0.001;

  return n / (f - z * (f - n)) * f;
}

vec3 DecodePositionFromDepth(vec2 texcoord)
{
  float depth = linearZ(texture(gBufferDepth, texcoord).r);
  vec4 clipSpaceLocation;
  clipSpaceLocation.xy = texcoord * 2.0 - 1.0;
  clipSpaceLocation.z = depth * 2.0 - 1.0;
  clipSpaceLocation.w = 1.0;
  vec4 homogenousLocation = viewProjectionInverseMatrix * clipSpaceLocation;
  return homogenousLocation.xyz / homogenousLocation.w;
}

vec3 DecodePositionFromDepth2(vec2 texcoord)
{
  float depth = texture(gBufferDepth, texcoord).r;
  vec4 clipSpaceLocation;
  clipSpaceLocation.xy = texcoord;
  clipSpaceLocation.z = depth;
  clipSpaceLocation.w = 1.0;
  vec4 homogenousLocation = viewProjectionInverseMatrix * clipSpaceLocation;
  return homogenousLocation.xyz / homogenousLocation.w;
}

vec3 myDecode(vec2 texCoord)
{
	vec4 screenSpacePosition = vec4(texCoord * 2.0 - 1.0, texture(gBufferDepth,     texCoord).r, 1);
	vec4 worldSpacePosition  = viewProjectionInverseMatrix  * screenSpacePosition;
	vec3 finalPosition       = worldSpacePosition.xyz / worldSpacePosition.w;
	
	return finalPosition;
}


vec3 VSPositionFromDepth(vec2 vTexCoord)
{
    // Get the depth value for this pixel
    float z = texture(gBufferDepth, vTexCoord).r;  
	
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = (1 - vTexCoord.y) * 2 - 1;
    vec4 vProjectedPos = vec4(x, y, z, 1.0f);
	
    // Transform by the inverse projection matrix
    vec4 vPositionVS = vProjectedPos * viewProjectionInverseMatrix;
	
    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;  
}


// new 
in vec2 ViewRay;
uniform mat4 gProj;

float CalcZPosition(vec2 texcoord)
{  
	float Depth = texture(gBufferDepth, texcoord).x;
	
	float ViewZ = gProj[3][2] / (2 * Depth -1 - gProj[2][2]);
	
	return ViewZ;
}

