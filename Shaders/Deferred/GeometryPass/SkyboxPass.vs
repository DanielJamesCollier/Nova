#version 450

layout (location = 0) in vec3 Position;

uniform mat4 MVP;

out vec3 fsPosition;

void main()
{
	fsPosition = Position;
 gl_Position = MVP * vec4(Position,1.0);
} 