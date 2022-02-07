#pragma once
#include <glm/ext/vector_int2.hpp>
#include "VertexContext.h"
class PixelIterator;
class FaceContext
{
public:
	glm::ivec2 screenPosition[3];
	float z[3];
	float w[3];
	VertexContext vertexContext[3];
	glm::ivec4 GetFaceScreenSize();
	PixelIterator GetStartPixelIterator();
	PixelIterator GetEndPixelIterator();
};

