#pragma once
#include <glm/glm.hpp>
#include "FaceContext.h"
#include <algorithm>
class PixelIterator {
    int x;
    int y;
    glm::ivec4 pole;
    glm::ivec2* screenPosition;
public:
    PixelIterator(FaceContext& faceContext);
    PixelIterator& operator++();
    PixelIterator operator++(int);
    bool CheckValid();
    glm::dvec3 GetBarycentricCoordinates();
    glm::ivec2 GetScreenPosition();
private:
    int Product(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3);
    bool CheckInTriangle(glm::ivec2 pos1, glm::ivec2 pos2, glm::ivec2 pos3, glm::ivec2 pos);
    bool CheckInTriangle(glm::ivec2 position);
    bool CheckInTriangle();
};

