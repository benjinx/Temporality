/*
	Pass Color
	By Benji Campbell
	Vertex shader that passes attributes down pipeline.
*/

#version 330 core

// attributes
layout (location = 0) in vec4 position;
layout (location = 8) in vec2 texCoord;

// uniforms
uniform mat4 mvp;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat2;

// varyings
out vec2 passTexCoord;

void main()
{
	gl_Position = projMat2 * viewMat * modelMat * position;
	passTexCoord = texCoord;
}