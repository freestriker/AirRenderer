#include "include/context/PixelIterator.h"
#include <algorithm>
#include <qdebug.h>

PixelIterator::PixelIterator(PrimitiveContext& primitiveContext, std::vector<VertexOutContext>& vertexOutContexts)
{
    //this->primitiveContext = &primitiveContext;
    this->primitiveType = primitiveContext.primitiveType;

    this->screenPosition[0] = vertexOutContexts[primitiveContext.vertexIndexes[0]].screenPosition;
    this->screenPosition[1] = vertexOutContexts[primitiveContext.vertexIndexes[1]].screenPosition;
    this->screenPosition[2] = vertexOutContexts[primitiveContext.vertexIndexes[2]].screenPosition;

    this->w[0] = vertexOutContexts[primitiveContext.vertexIndexes[0]].w;
    this->w[1] = vertexOutContexts[primitiveContext.vertexIndexes[1]].w;
    this->w[2] = vertexOutContexts[primitiveContext.vertexIndexes[2]].w;

    this->pole = GetFacePole(this->screenPosition);
    this->x = this->pole.x;
    this->y = this->pole.y;

    int i = -1, j = -1;
    for (i = pole.x; i <= pole.z; i++)
    {
        for (j = pole.y; j <= pole.w; j++)
        {
            if (CheckInTriangle(glm::vec2(i, j)))
            {
                x = i;
                y = j;
                goto Out;
            }
        }
    }
Out:
    x = i;
    y = j;

}
glm::ivec4 PixelIterator::GetFacePole(glm::vec3* screenPosition)
{
    int index1 = 0, index2 = 0;
    switch (primitiveType)
    {
        case PrimitiveType::POINT:
        {

            break;
        }
        case PrimitiveType::LINE:
        {
            index1 = 1;
            break;
        }
        case PrimitiveType::TRIANGLE:
        {
            index1 = 1;
            index2 = 2;
            break;
        }
    }
    
    int minX = std::min(screenPosition[0].x, std::min(screenPosition[index1].x, screenPosition[index2].x));
    int minY = std::min(screenPosition[0].y, std::min(screenPosition[index1].y, screenPosition[index2].y));
    int maxX = std::max(screenPosition[0].x, std::max(screenPosition[index1].x, screenPosition[index2].x));
    int maxY = std::max(screenPosition[0].y, std::max(screenPosition[index1].y, screenPosition[index2].y));
    return glm::ivec4(minX, minY, maxX, maxY);
}
PixelIterator& PixelIterator::operator++()
{
    int i =-1, j = -1;
    for (i = x; i <= pole.z; i++)
    {
        for (j = i == x ? y + 1 : pole.y; j <= pole.w; j++)
        {
            if (CheckInTriangle(glm::vec2(i, j)))
            {
                x = i;
                y = j;
                return *this;
            }
        }
    }
    x = i;
    y = j;
    return *this;
}
PixelIterator PixelIterator::operator++(int)
{
    int i = -1, j = -1;
    for (i = x; i <= pole.z; i++)
    {
        for (j = i == x ? y + 1 : pole.y; j <= pole.w; j++)
        {
            if (CheckInTriangle(glm::vec2(i, j)))
            {
                x = i;
                y = j;
                return *this;
            }
        }
    }
    x = i;
    y = j;
    return *this;
}
bool PixelIterator::CheckValid()
{
    return pole.x <= x && x <= pole.z 
        && pole.y <= y && y <= pole.w;
}

float PixelIterator::Product(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    //首先根据坐标计算p1p2和p1p3的向量，然后再计算叉乘
    //p1p2 向量表示为 (p2.x-p1.x,p2.y-p1.y)
    //p1p3 向量表示为 (p3.x-p1.x,p3.y-p1.y)
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

bool PixelIterator::CheckInTriangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 pos3, glm::vec2 pos)
{
    if (Product(pos1, pos2, pos3) < 0.0) return CheckInTriangle(pos1, pos3, pos2, pos);
    if (Product(pos1, pos2, pos) >= 0.0 && Product(pos2, pos3, pos) >= 0.0 && Product(pos3, pos1, pos) >= 0.0)
        return true;
    return false;
}bool PixelIterator::CheckInTriangle(glm::vec2 position)
{
    switch (primitiveType)
    {
        case PrimitiveType::POINT:
        {
            return true;
        }
        case PrimitiveType::LINE:
        {
            
            float d = glm::dot(glm::normalize(glm::vec2(screenPosition[0]) - position), glm::normalize(glm::vec2(screenPosition[1]) - position));
            //qDebug() << d;
            return -1.001 <= d && d <= -0.999;
        }
        case PrimitiveType::TRIANGLE:
        {
            return CheckInTriangle(screenPosition[0], screenPosition[1], screenPosition[2], position);
        }
    }
}

glm::dvec3 PixelIterator::GetInterpolationCoefficient(CameraContext* cameraContext)
{
    switch (primitiveType)
    {
    case PrimitiveType::POINT:
    {
        return glm::dvec3(1.0, 0, 0);
    }
    case PrimitiveType::LINE:
    {
        if (pole.z - pole.x == 0)
        {
            double r2 = double(y - pole.y) / double(pole.w - pole.y);
            r2 = std::clamp(r2, 0.0, 1.0);
            double r1 = std::clamp(1.0 - r2, 0.0, 1.0);
            if (cameraContext->needPerspectiveCorrection)
            {
                double zn = 1 / (r1 / this->w[0] + r2 / this->w[1]);
                r1 = std::clamp(zn * r1 / this->w[0], 0.0, 1.0);
                r2 = std::clamp(1.0 - r1, 0.0, 1.0);
            }
            return glm::dvec3(r1, r2, 0);
        }
        else
        {
            double r1 = double(x - pole.x) / double(pole.z - pole.x);
            r1 = std::clamp(r1, 0.0, 1.0);
            double r2 = std::clamp(1.0 - r1, 0.0, 1.0);
            if (cameraContext->needPerspectiveCorrection)
            {
                double zn = 1 / (r1 / this->w[0] + r2 / this->w[1]);
                r1 = std::clamp(zn * r1 / this->w[0], 0.0, 1.0);
                r2 = std::clamp(1.0 - r1, 0.0, 1.0);
            }
            return glm::dvec3(r1, r2, 0);
        }
    }
    case PrimitiveType::TRIANGLE:
    {
        glm::vec2 pos1 = screenPosition[0];
        glm::vec2 pos2 = screenPosition[1];
        glm::vec2 pos3 = screenPosition[2];
        double ratio1 = double(-(x - pos2.x) * (pos3.y - pos2.y) + (y - pos2.y) * (pos3.x - pos2.x))
            / double(-(pos1.x - pos2.x) * (pos3.y - pos2.y) + (pos1.y - pos2.y) * (pos3.x - pos2.x));
        ratio1 = std::clamp(ratio1, 0.0, 1.0);
        double ratio2 = double(-(x - pos3.x) * (pos1.y - pos3.y) + (y - pos3.y) * (pos1.x - pos3.x))
            / double(-(pos2.x - pos3.x) * (pos1.y - pos3.y) + (pos2.y - pos3.y) * (pos1.x - pos3.x));
        ratio2 = std::clamp(ratio2, 0.0, 1.0);
        double ratio3 = 1.0 - ratio1 - ratio2;
        ratio3 = std::clamp(ratio3, 0.0, 1.0);

        if (cameraContext->needPerspectiveCorrection)
        {
            double zn = 1 / (ratio1 / this->w[0] + ratio2 / this->w[1] + ratio3 / this->w[2]);
            ratio1 = std::clamp(zn * ratio1 / this->w[0], 0.0, 1.0);
            ratio2 = std::clamp(zn * ratio2 / this->w[1], 0.0, 1.0);
            ratio3 = std::clamp(1.0 - ratio1 - ratio2, 0.0, 1.0);
        }

        return glm::dvec3(ratio1, ratio2, ratio3);
    }
    }
}
glm::ivec2 PixelIterator::GetScreenPosition()
{
    return glm::ivec2(x, y);
}