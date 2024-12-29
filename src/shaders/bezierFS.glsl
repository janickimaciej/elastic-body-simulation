#version 420 core

in vec2 texturePos;

uniform sampler2D textureSampler;

out vec4 outColor;

void main()
{
	vec3 textureColor = vec3(texture(textureSampler, texturePos));
	outColor = vec4(textureColor, 0.8f);
}
