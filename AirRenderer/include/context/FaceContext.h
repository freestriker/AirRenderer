#pragma once
#include <glm/ext/vector_int2.hpp>
#include <glm/vec4.hpp>
#include <algorithm>
class FaceContext
{
public:
	glm::ivec2 screenPosition[3];
	//float z[3];
	float w[3];
	int vertexIndex[3];
	glm::ivec4 GetFacePole();
};

