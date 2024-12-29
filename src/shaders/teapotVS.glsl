#version 420 core

layout (location = 0) in vec3 inPos;

uniform vec3 bezierPoints[64];
uniform mat4 projectionViewMatrix;

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
	gl_Position = projectionViewMatrix * vec4(deCasteljau4(bezierPointsUV[0], bezierPointsUV[1],
		bezierPointsUV[2], bezierPointsUV[3], inPos.z), 1);
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
