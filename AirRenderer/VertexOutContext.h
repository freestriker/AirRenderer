#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Color.h"
class VertexOutContext
{
public:
	glm::vec4 pPosition;
	glm::vec4 vPosition;
	glm::vec4 wPosition;
	glm::vec3 vNormal;
	glm::vec3 wNormal;
	glm::vec3 vTangent;
	glm::vec3 wTangent;
	glm::vec2 texcoord1;
	Color color;
};

