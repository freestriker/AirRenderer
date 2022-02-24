#pragma once
#include <glm/ext/vector_int2.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <algorithm>
class FaceContext
{
public:
	glm::vec3 screenPosition[3];
	//float z[3];
	float w[3];
	int vertexIndex[3];
	glm::ivec4 GetFacePole();
};

