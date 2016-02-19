#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 texcoord;

uniform mat4 MVP;

void main() 
{
    gl_Position = MVP * vec4( 1.0, 1.0, 0.0, 1.0 );
    texcoord = vec2( 1.0, 1.0 );
    EmitVertex();

    gl_Position = MVP * vec4(-1.0, 1.0, 0.0, 1.0 );
    texcoord = vec2( 0.0, 1.0 ); 
    EmitVertex();

    gl_Position = MVP * vec4( 1.0,-1.0, 0.0, 1.0 );
    texcoord = vec2( 1.0, 0.0 ); 
    EmitVertex();

    gl_Position = MVP * vec4(-1.0,-1.0, 0.0, 1.0 );
    texcoord = vec2( 0.0, 0.0 ); 
    EmitVertex();

    EndPrimitive(); 
}