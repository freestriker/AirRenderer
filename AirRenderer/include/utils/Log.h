#pragma once
#include <glm/mat4x4.hpp>
#include <string>
class Log
{
public:
    static void LogMatrix(std::string name, glm::mat4 matrix);
    static void LogVector(std::string name, glm::vec4 vector);
};

