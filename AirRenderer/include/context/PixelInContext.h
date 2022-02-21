#pragma once
#include <glm/ext/vector_int2.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "include/utils/Color.h"
class PixelInContext
{
public:
    glm::ivec2 screenPosition;
    float z;
    float w;
    glm::vec4 data[8]{ glm::vec4(0, 0, 0, 0) };
};

