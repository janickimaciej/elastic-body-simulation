#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormalVector;

uniform vec3 bezierPoints[64];
uniform mat4 projectionViewMatrix;

out vec3 pos;
out vec3 normalVector;

int index(int ui, int vi, int wi);
vec3 deCasteljau2(vec3 a, vec3 b, float t);
vec3 deCasteljau3(vec3 a, vec3 b, vec3 c, float t);
vec3 deCasteljau4(vec3 a, vec3 b, vec3 c, vec3 d, float t);
vec3 deCasteljauDeriv4(vec3 a, vec3 b, vec3 c, vec3 d, float t);

void main()
{
	vec3 bezierPointsU[16];
	for (int wi = 0; wi < 4; ++wi)
	{
		for (int vi = 0; vi < 4; ++vi)
		{
			bezierPointsU[4 * wi + vi] = deCasteljau4(bezierPoints[index(0, vi, wi)],
				bezierPoints[index(1, vi, wi)], bezierPoints[index(2, vi, wi)],
				bezierPoints[index(3, vi, wi)], inPos.x);
		}
	}

	vec3 bezierPointsV[16];
	for (int ui = 0; ui < 4; ++ui)
	{
		for (int wi = 0; wi < 4; ++wi)
		{
			bezierPointsV[4 * ui + wi] = deCasteljau4(bezierPoints[index(ui, 0, wi)],
				bezierPoints[index(ui, 1, wi)], bezierPoints[index(ui, 2, wi)],
				bezierPoints[index(ui, 3, wi)], inPos.y);
		}
	}

	vec3 bezierPointsW[16];
	for (int vi = 0; vi < 4; ++vi)
	{
		for (int ui = 0; ui < 4; ++ui)
		{
			bezierPointsW[4 * vi + ui] = deCasteljau4(bezierPoints[index(ui, vi, 0)],
				bezierPoints[index(ui, vi, 1)], bezierPoints[index(ui, vi, 2)],
				bezierPoints[index(ui, vi, 3)], inPos.z);
		}
	}

	vec3 bezierPointsUV[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierPointsUV[i] = deCasteljau4(bezierPointsU[4 * i], bezierPointsU[4 * i + 1],
			bezierPointsU[4 * i + 2], bezierPointsU[4 * i + 3], inPos.y);
	}

	vec3 bezierPointsVW[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierPointsVW[i] = deCasteljau4(bezierPointsV[4 * i], bezierPointsV[4 * i + 1],
			bezierPointsV[4 * i + 2], bezierPointsV[4 * i + 3], inPos.z);
	}

	vec3 bezierPointsWU[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierPointsWU[i] = deCasteljau4(bezierPointsW[4 * i], bezierPointsW[4 * i + 1],
			bezierPointsW[4 * i + 2], bezierPointsW[4 * i + 3], inPos.x);
	}

	pos = deCasteljau4(bezierPointsUV[0], bezierPointsUV[1], bezierPointsUV[2],
		bezierPointsUV[3], inPos.z);
	gl_Position = projectionViewMatrix * vec4(pos, 1);

	vec3 jacobianU = deCasteljauDeriv4(bezierPointsVW[0], bezierPointsVW[1], bezierPointsVW[2],
		bezierPointsVW[3], inPos.x);
	vec3 jacobianV = deCasteljauDeriv4(bezierPointsWU[0], bezierPointsWU[1], bezierPointsWU[2],
		bezierPointsWU[3], inPos.y);
	vec3 jacobianW = deCasteljauDeriv4(bezierPointsUV[0], bezierPointsUV[1], bezierPointsUV[2],
		bezierPointsUV[3], inPos.z);
	mat3 jacobian = mat3(jacobianU, jacobianV, jacobianW);
	normalVector = inverse(transpose(jacobian)) * inNormalVector;
}

int index(int ui, int vi, int wi)
{
	return 16 * wi + 4 * vi + ui;
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

vec3 deCasteljauDeriv4(vec3 a, vec3 b, vec3 c, vec3 d, float t)
{
	return 3 * deCasteljau3(-a + b, -b + c, -c + d, t);
}
