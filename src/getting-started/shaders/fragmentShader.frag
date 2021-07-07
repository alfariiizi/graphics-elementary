#version 430 core

layout( location = 0 ) in vec4 fragColor;
layout( location = 1 ) in vec2 texcoord;

layout( location = 0 ) out vec4 outColor;

layout( location = 0 ) uniform sampler2D containerTex;
layout( location = 1 ) uniform sampler2D awesomefaceTex;

void main()
{
    // ourTexture: 2D ; texcoord vec2 ; so, the total maybe 4 float (vec4)
    outColor = mix( texture(containerTex, texcoord), texture(awesomefaceTex, texcoord), 0.2 );
}