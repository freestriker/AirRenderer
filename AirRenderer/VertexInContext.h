#pragma once
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "Color.h"
class VertexInContext
{
public:
	glm::vec4 position;
	glm::vec2 texcoord1;
	Color color;

	glm::mat4 mvpMatrix;
	int vertexIndex;
};

