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
    glm::vec4 worldPosition;
    glm::vec3 normal;
    Color color;
    glm::vec2 texcoord1;
};

