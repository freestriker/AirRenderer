#pragma once
#include <glm/ext/vector_int2.hpp>
#include <QImage>
#include "Configuration.h"

using namespace glm;

class Drawer
{
public:
	static void DrawLine(ivec2 startPosition, ivec2 endPosition, QRgb color);
	static void DrawTriangle(ivec2 positionA, ivec2 positionB, ivec2 positionC, QRgb color);
};

