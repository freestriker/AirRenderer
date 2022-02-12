#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Color.h"
class VertexOutContext
{
public:
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 texcoord1;
	Color color;
};

