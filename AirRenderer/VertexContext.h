#pragma once
#include <glm/vec4.hpp>
#include "Color.h"
class VertexContext
{
public:
	glm::vec4 position;
	Color color;
	VertexContext();
	VertexContext(glm::vec4 position, Color color);
};

