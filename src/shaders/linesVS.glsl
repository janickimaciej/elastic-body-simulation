#version 420 core

layout (location = 0) in vec3 inPos;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;
uniform bool depthOffset;

void main()
{
	gl_Position = projectionViewMatrix * modelMatrix * vec4(inPos, 1);
	if (depthOffset)
	{
		gl_Position += vec4(0, 0, -0.001f, 0);
	}
}
