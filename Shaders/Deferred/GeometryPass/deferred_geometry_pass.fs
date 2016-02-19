#version 450 core

// inputs from vertex shader
in vec3 fsWorldPos;
in vec4 fsColour;
in vec2 fsTexCoord;
in vec3 fsNormal;
in vec3 fsTangent;

// if textureCount equals 4 there must be 4 diffuse, 4 normal, and 4 specular // 4 * 3 = 12 textures
uniform int textureCount;

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec4 AlbedoSpecOut; 
layout (location = 2) out vec3 NormalOut; 

// 0 = albedo | 1 = normal | 2 = specular
layout(binding = 0) uniform sampler2DArray material;

vec3 CalcBumpedNormal(vec3 n)
 {
     vec3 Normal = normalize(fsNormal);
     vec3 Tangent = normalize(fsTangent);
     Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
     vec3 Bitangent = cross(Tangent, Normal);
	 vec3 BumpMapNormal = 2.0 * n - vec3(1.0, 1.0, 1.0);
     vec3 NewNormal;
     mat3 TBN = mat3(Tangent, Bitangent, Normal);
     NewNormal = TBN * BumpMapNormal;
     NewNormal = normalize(NewNormal);
	 
	 NewNormal *= 2.0 - 1.0;
     return NewNormal;
 }

void main() 
{ 
     WorldPosOut = fsWorldPos; 
	 
	 vec3 albedo = texture(material,vec3(fsTexCoord,0)).rgb;
	 vec3 normal = texture(material,vec3(fsTexCoord,1)).rgb;
	 float spec  = texture(material,vec3(fsTexCoord,2)).r;
	 
     AlbedoSpecOut  = vec4(albedo, spec); 
	 NormalOut      = CalcBumpedNormal(normal);  // normal mapped normal
	// NormalOut = normal; // normals without bump mapping
 } 