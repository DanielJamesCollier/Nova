#version 450

in vec3 fsPosition;

out vec4 FragColor;

layout (binding = 0) uniform samplerCube cubemap;

void main()
{
 FragColor = texture(cubemap, fsPosition);

} 