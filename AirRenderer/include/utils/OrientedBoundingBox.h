#pragma once
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <qdebug.h>
#include <algorithm>
class OrientedBoundingBox
{
public:
    glm::mat3 computeCovarianceMatrix(glm::vec3* pVertices, int numVertices);
    void jacobiSolver(glm::mat3& matrix, float* eValue, glm::vec3* eVectors);
    void schmidtOrthogonal(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2);
};