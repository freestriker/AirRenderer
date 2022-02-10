#pragma once
#include "VertexOutContext.h"
#include <glm/ext/vector_int2.hpp>
#include "Color.h"
class PixelInContext
{
public:
    glm::ivec2 screenPosition;
    float z;
    float w;
    glm::vec4 position;
    Color color;
    glm::vec2 texcoord1;
};

