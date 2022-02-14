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
    glm::vec4 pPosition;
    glm::vec3 vTangent;
    glm::vec3 wTangent;

    glm::vec4 vPosition;
    glm::vec4 wPosition;
    glm::vec3 vNormal;
    glm::vec3 wNormal;
    Color color;
    glm::vec2 texcoord1;
};

