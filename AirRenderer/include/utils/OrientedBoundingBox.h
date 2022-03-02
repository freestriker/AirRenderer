#pragma once
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <qdebug.h>
#include <algorithm>
#include <include/core_object/Mesh.h>
class OrientedBoundingBox
{
    glm::vec3 center;
    glm::vec3 directions[3];
    glm::vec3 halfEdgeLength;
public:
    static glm::mat3 computeCovarianceMatrix(ModelMesh& modelMesh);
    static void jacobiSolver(glm::mat3& matrix, float* eValue, glm::vec3* eVectors);
    static void schmidtOrthogonal(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2);
    OrientedBoundingBox(ModelMesh& modelMesh);
};