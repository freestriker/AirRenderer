#pragma once
#include <glm/mat4x4.hpp>
#include <qdebug.h>
static class Utils
{
public:
    static void LogMatrix(std::string name, glm::mat4 matrix);
};

