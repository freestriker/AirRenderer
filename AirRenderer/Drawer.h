#pragma once
#include <glm/ext/vector_int2.hpp>
#include "Color.h"
#include "Buffer.h"

using namespace glm;

class Drawer
{
public:
	static void DrawLine(ivec2 startPosition, ivec2 endPosition, Color color, Buffer<Color>& buffer);
	static void DrawTriangle(ivec2 positionA, ivec2 positionB, ivec2 positionC, Color color, Buffer<Color>& buffer);
};

