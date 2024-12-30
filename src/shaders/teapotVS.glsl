#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormalVector;

uniform vec3 bezierPoints[64];
uniform mat4 projectionViewMatrix;

out vec3 pos;
out vec3 normalVector;

vec3 deCasteljau2(vec3 a, vec3 b, float t);
vec3 deCasteljau3(vec3 a, vec3 b, vec3 c, float t);
vec3 deCasteljau4(vec3 a, vec3 b, vec3 c, vec3 d, float t);

void main()
{
	vec3 bezierPointsU[16];
	for (int i = 0; i < 16; ++i)
	{
		bezierPointsU[i] = deCasteljau4(bezierPoints[4 * i], bezierPoints[4 * i + 1],
			bezierPoints[4 * i + 2], bezierPoints[4 * i + 3], inPos.x);
	}
	vec3 bezierPointsUV[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierPointsUV[i] = deCasteljau4(bezierPointsU[4 * i], bezierPointsU[4 * i + 1],
			bezierPointsU[4 * i + 2], bezierPointsU[4 * i + 3], inPos.y);
	}
	pos = deCasteljau4(bezierPointsUV[0], bezierPointsUV[1], bezierPointsUV[2],
		bezierPointsUV[3], inPos.z);
	gl_Position = projectionViewMatrix * vec4(pos, 1);
	normalVector = inNormalVector;
}

vec3 deCasteljau2(vec3 a, vec3 b, float t)
{
	return mix(a, b, t);
}

vec3 deCasteljau3(vec3 a, vec3 b, vec3 c, float t)
{
	return deCasteljau2(mix(a, b, t), mix(b, c, t), t);
}

vec3 deCasteljau4(vec3 a, vec3 b, vec3 c, vec3 d, float t)
{
	return deCasteljau3(mix(a, b, t), mix(b, c, t), mix(c, d, t), t);
}
