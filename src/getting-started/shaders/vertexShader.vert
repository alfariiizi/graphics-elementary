#version 430 core

layout( location = 0 ) in vec3 aPos;
layout( location = 1 ) in vec3 aColor;

layout( location = 0 ) out vec4 fragColor;

layout( location = 0 ) uniform float u_colorChangeable;

void main()
{
    gl_Position = vec4( aPos, 1.0f );
    fragColor = vec4( aColor.xy, aColor.z + u_colorChangeable, 1.0f );
}