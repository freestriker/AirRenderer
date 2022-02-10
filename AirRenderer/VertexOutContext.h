#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "Color.h"
class VertexOutContext
{
public:
	glm::vec4 position;
	glm::vec2 texcoord1;
	Color color;
};

