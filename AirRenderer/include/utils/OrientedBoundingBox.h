#pragma once
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <qdebug.h>
#include <algorithm>
class ModelMesh;
class OrientedBoundingBox
{
public:
    glm::vec3 center;
    glm::vec3 directions[3];
    glm::vec3 halfEdgeLength;
    glm::vec3 boundryVertexes[8];
    static glm::mat3 computeCovarianceMatrix(ModelMesh& modelMesh);
    static void jacobiSolver(glm::mat3& matrix, float* eValue, glm::vec3* eVectors);
    static void schmidtOrthogonal(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2);
    void BuildBoundingBox(ModelMesh& modelMesh);
    OrientedBoundingBox(ModelMesh& modelMesh);
    OrientedBoundingBox();
};