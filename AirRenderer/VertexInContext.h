#pragma once
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "Color.h"
class VertexInContext
{
public:
	glm::vec4 mPosition;
	glm::vec3 mNormal;
	glm::vec3 mTangent;
	glm::vec2 texcoord1;
	Color color;

	int vertexIndex;
};

