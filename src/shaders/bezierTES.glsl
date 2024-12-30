#version 420 core

#define meshDensity 16

layout (quads, equal_spacing, ccw) in;
in vec3 tessPos[];

uniform mat4 projectionViewMatrix;

out vec3 pos;
out vec2 texturePos;
out vec3 normalVector;

vec3 deCasteljau2(vec3 a, vec3 b, float t);
vec3 deCasteljau3(vec3 a, vec3 b, vec3 c, float t);
vec3 deCasteljau4(vec3 a, vec3 b, vec3 c, vec3 d, float t);
vec3 deCasteljauDeriv4(vec3 a, vec3 b, vec3 c, vec3 d, float t);

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	texturePos = vec2(u / 2.0f, v / 2.0f);

	vec3 bezierU[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierU[i] = deCasteljau4(tessPos[4 * i], tessPos[4 * i + 1], tessPos[4 * i + 2],
			tessPos[4 * i + 3], u);
	}

	vec3 bezierV[4];
	for (int i = 0; i < 4; ++i)
	{
		bezierV[i] = deCasteljau4(tessPos[i], tessPos[i + 4], tessPos[i + 8], tessPos[i + 12], v);
	}

	pos = deCasteljau4(bezierU[0], bezierU[1], bezierU[2], bezierU[3], v);
	gl_Position = projectionViewMatrix * vec4(pos, 1);

	vec3 derivU = deCasteljauDeriv4(bezierV[0], bezierV[1], bezierV[2], bezierV[3], u);
	vec3 derivV = deCasteljauDeriv4(bezierU[0], bezierU[1], bezierU[2], bezierU[3], v);
	normalVector = cross(derivU, derivV);
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
