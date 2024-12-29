#version 420 core

#define meshDensity 16

layout (quads, equal_spacing, ccw) in;
in vec3 tessPos[];

uniform mat4 projectionViewMatrix;

out vec2 texturePos;

vec3 deCasteljau2(vec3 a, vec3 b, float t);
vec3 deCasteljau3(vec3 a, vec3 b, vec3 c, float t);
vec3 deCasteljau4(vec3 a, vec3 b, vec3 c, vec3 d, float t);

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	texturePos = vec2(u / 2.0f, v / 2.0f);

	vec3 bezierV[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierV[i] = deCasteljau4(tessPos[4 * i], tessPos[4 * i + 1], tessPos[4 * i + 2],
			tessPos[4 * i + 3], u);
	}
	gl_Position = projectionViewMatrix * vec4(deCasteljau4(bezierV[0], bezierV[1], bezierV[2],
		bezierV[3], v), 1);
}

vec3 deCasteljau2(vec3 a, vec3 b, float t)
{
	return mix(a, b, t);
}

vec3 deCasteljau3(vec3 a, vec3 b, vec3 c, float t)
{
	return mix(deCasteljau2(a, b, t), deCasteljau2(b, c, t), t);
}

vec3 deCasteljau4(vec3 a, vec3 b, vec3 c, vec3 d, float t)
{
	return mix(deCasteljau3(a, b, c, t), deCasteljau3(b, c, d, t), t);
}
