#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
	TexCoords = position;
	gl_Position = projMat * viewMat * vec4(position, 1.0);
}