#include "VertexContext.h"

VertexContext::VertexContext():VertexContext(glm::vec4(0, 0, 0, 1), Color(0, 0, 0, 0))
{
}

VertexContext::VertexContext(glm::vec4 position, Color color)
{
	this->position = position;
	this->color = color;
}
