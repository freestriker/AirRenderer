#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <include/context/CameraContext.h>
#include <include/context/PrimitiveContext.h>
class PixelIterator {
    int x;
    int y;
    glm::vec4 pole;
    glm::ivec4 roundPole;
    glm::vec3 screenPosition[3];
    PrimitiveType primitiveType;
    float w[3];

    bool isSteep = false;
    glm::ivec2 difference;
    glm::ivec2 startPosition, endPosition;
    int deltaAbs2X;
    int deltaAbs2Y;
    int d;
    int dy;
    int lineX, lineY;


    //PrimitiveContext* primitiveContext;
public:
    PixelIterator(PrimitiveContext& primitiveContext, std::vector<VertexOutContext>& vertexOutContexts);
    glm::ivec4 GetFacePole(glm::vec3* screenPosition);
    PixelIterator& operator++();
    PixelIterator operator++(int);
    bool CheckValid();
    glm::dvec3 GetInterpolationCoefficient(CameraContext* cameraContext);
    glm::ivec2 GetScreenPosition();
private:
    float Product(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
    bool CheckInTriangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 pos3, glm::vec2 pos);
    bool CheckInTriangle(glm::vec2 position);
    void Init();
    void InitTriangle();
    void InitLine();
    void InitPoint();
    void TriangleNext();
    void LineNext();
    void PointNext();
    void Next();
    bool PixelIterator::CheckTriangleValid();
    bool PixelIterator::CheckLineValid();
    bool PixelIterator::CheckPointValid();
};

