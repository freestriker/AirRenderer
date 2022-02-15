#pragma once
#include <glm/mat4x4.hpp>
#include <string>
static class Log
{
public:
    static void LogMatrix(std::string name, glm::mat4 matrix);
};

