#version 400 core

layout (location = 0) in vec3 inPos;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

void main()
{
	gl_Position = projectionViewMatrix * modelMatrix * vec4(inPos, 1);
}
