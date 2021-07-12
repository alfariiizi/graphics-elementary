#version 430 core

layout( location = 0 ) in vec3 aPos;
layout( location = 1 ) in vec3 aColor;
layout( location = 2 ) in vec2 aTexcoord;

layout( location = 0 ) out vec4 fragColor;
layout( location = 1 ) out vec2 texcoord;

// layout( location = 0 ) uniform float u_colorChangeable;
layout( location = 4 ) uniform mat4 transform;
layout( location = 5 ) uniform mat4 model;
layout( location = 6 ) uniform mat4 view;
layout( location = 7 ) uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4( aPos, 1.0f );
    fragColor = vec4( aColor, 1.0f );
    texcoord = aTexcoord;
}