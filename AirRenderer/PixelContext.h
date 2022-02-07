#pragma once
#include "VertexContext.h"
#include <glm/ext/vector_int2.hpp>
class PixelContext
{
public:
    glm::ivec2 screenPosition;
    float z;
    float w;
    glm::vec4 position;
    Color color;
};

