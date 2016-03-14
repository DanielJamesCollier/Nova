#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out vec3 fsWorldPos;
out vec4 fsColour;
out vec2 fsTexCoord;
out vec3 fsNormal;
out vec3 fsTangent;


uniform mat4 MVP;
uniform mat4 Model;

void main()
{
	fsTexCoord  = vec2(texCoord.x, 1 - texCoord.y); // flip the y on uv's
	fsNormal    = (Model * vec4(normal,0.0)).xyz;   // convert normal to world space
	fsWorldPos  = (Model * vec4(position,1.0)).xyz; // convert position to world space
	fsTangent   = (Model * vec4(tangent,0.0)).xyz;  // convert tangent to world space

	gl_Position = MVP * vec4(position,1.0);
}
