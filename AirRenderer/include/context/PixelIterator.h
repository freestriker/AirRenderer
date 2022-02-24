#pragma once
#include <glm/glm.hpp>
#include "include/context/FaceContext.h"
#include <include/context/CameraContext.h>
class PixelIterator {
    int x;
    int y;
    glm::vec4 pole;
    glm::vec3* screenPosition;
    FaceContext* faceContext;
public:
    PixelIterator(FaceContext& faceContext);
    PixelIterator& operator++();
    PixelIterator operator++(int);
    bool CheckValid();
    glm::dvec3 GetInterpolationCoefficient(CameraContext* cameraContext);
    glm::ivec2 GetScreenPosition();
private:
    float Product(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
    bool CheckInTriangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 pos3, glm::vec2 pos);
    bool CheckInTriangle(glm::vec2 position);
    bool CheckInTriangle();
};

