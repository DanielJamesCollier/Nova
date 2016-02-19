#version 450 core

#IncludeGuard "Shaders/Common/PointLight.glsl"
#IncludeGuard "Shaders/Common/DirectionalLight.glsl"
#IncludeGuard "Shaders/Common/SpotLight.glsl"

// inputs from vertex shader
in vec3 fsWorldPos;
in vec4 fsColour;
in vec2 fsTexCoord;
in vec3 fsNormal;

// screen frag output
out vec4 colour; 

// inputs from application
uniform sampler2D  diffuse;

void main()
{
	vec3 normal = normalize(fsNormal);
	
	vec4 totalLight;
	
	for(int i = 0; i < numDirLights; i++)
	{
		totalLight += CalcDirectionLight(dLights[i], normal, fsWorldPos);
	}
	
	for (int i = 0 ; i < numPointLights; i++) 
	{
         totalLight += CalcPointLight(pLights[i], normal, fsWorldPos);
    }
	
	for(int i = 0; i < numSpotLights; i++)
	{
		totalLight += CalcSpotLight(sLights[i],normal, fsWorldPos);
	}
	
	colour =  texture2D(diffuse,fsTexCoord) * totalLight;
}