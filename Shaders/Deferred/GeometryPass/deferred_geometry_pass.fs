#version 450 core

// inputs from vertex shader
in vec3 fsWorldPos;
in vec2 fsTexCoord;
in vec3 fsNormal;
in vec3 fsTangent;

// GBuffer outputs
layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec4 AlbedoSpecOut;
layout (location = 2) out vec3 NormalOut;

// 0 = albedo | 1 = normal | 2 = specular
layout(binding = 5) uniform sampler2DArray material;

vec3 CalcBumpedNormal()
 { 
     // normalize inputs
     vec3 inNormal      = normalize(fsNormal);
     vec3 inTangent     = normalize(fsTangent);
     // get normal map normal and scale it from (0 to 1) range to (-1 to 1) range
     vec3 BumpMapNormal = 2.0 * normalize(texture(material,vec3(fsTexCoord,1)).rgb) - vec3(1.0, 1.0, 1.0);
     // generate bitangent from tangent and normal
     vec3 Bitangent     = cross(inTangent, inNormal);
   	
     // Create TBN matrix to convert tangent space normal to world space normal
     mat3 TBN = mat3(inTangent, Bitangent, inNormal);

     vec3 NewNormal;
     NewNormal = normalize(TBN * BumpMapNormal);
     NewNormal *= 2.0 - 1.0;
     return NewNormal;
 }

void main()
{
     vec3 albedo = texture(material,vec3(fsTexCoord,0)).rgb;
     float spec  = texture(material,vec3(fsTexCoord,2)).r;
     
     // outputs
     WorldPosOut    = fsWorldPos;
     NormalOut      = CalcBumpedNormal();
     AlbedoSpecOut  = vec4(albedo, spec);

   //NormalOut = normal; // normals without bump mapping
 }