#pragma once
#include <glm/glm.hpp>
#include "PixelContext.h"
class FaceContext;
class PixelIterator {
    int x;
    int y;
    glm::ivec4 size;
    FaceContext* faceContext;
public:
    PixelIterator(FaceContext* faceContext, bool start);
    PixelIterator& operator++();
    PixelIterator operator++(int);
    bool operator==(PixelIterator other);
    bool operator!=(PixelIterator other);
    PixelContext operator*();
    int Product(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3);
    bool CheckInTriangle(glm::ivec2 pos1, glm::ivec2 pos2, glm::ivec2 pos3, glm::ivec2 pos);
    bool CheckInTriangle();
};

