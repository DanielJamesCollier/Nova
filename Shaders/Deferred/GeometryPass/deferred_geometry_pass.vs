#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;
layout (location = 4) in vec3 tangent;

out vec3 fsWorldPos;
out vec4 fsColour;
out vec2 fsTexCoord;
out vec3 fsNormal;
out vec3 fsTangent;


uniform mat4 MVP;
uniform mat4 Model;

void main()
{
	
	fsColour   = colour;
	fsTexCoord = vec2(texCoord.x, 1 - texCoord.y);
	fsNormal   = (Model * vec4(normal,0.0)).xyz;
	fsWorldPos = (Model * vec4(position,1.0)).xyz;
	fsTangent  = (Model * vec4(tangent,0.0)).xyz;
	
	gl_Position = MVP * vec4(position,1.0);
}
