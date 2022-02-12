#include "PixelIterator.h"

PixelIterator::PixelIterator(FaceContext& faceContext)
{
    this->screenPosition = faceContext.screenPosition;
    this->pole = faceContext.GetFacePole();
    this->x = this->pole.x;
    this->y = this->pole.y;

    int i = -1, j = -1;
    for (i = x; i <= pole.z; i++)
    {
        for (j = i == x ? y + 1 : pole.y; j <= pole.w; j++)
        {
            if (CheckInTriangle(glm::ivec2(i, j)))
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
PixelIterator& PixelIterator::operator++() 
{
    int i =-1, j = -1;
    for (i = x; i <= pole.z; i++)
    {
        for (j = i == x ? y + 1 : pole.y; j <= pole.w; j++)
        {
            if (CheckInTriangle(glm::ivec2(i, j)))
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
            if (CheckInTriangle(glm::ivec2(i, j)))
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

int PixelIterator::Product(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3) {
    //首先根据坐标计算p1p2和p1p3的向量，然后再计算叉乘
    //p1p2 向量表示为 (p2.x-p1.x,p2.y-p1.y)
    //p1p3 向量表示为 (p3.x-p1.x,p3.y-p1.y)
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

bool PixelIterator::CheckInTriangle(glm::ivec2 pos1, glm::ivec2 pos2, glm::ivec2 pos3, glm::ivec2 pos)
{
    if (Product(pos1, pos2, pos3) < 0) return CheckInTriangle(pos1, pos3, pos2, pos);
    if (Product(pos1, pos2, pos) >= 0 && Product(pos2, pos3, pos) >= 0 && Product(pos3, pos1, pos) >= 0)
        return true;
    return false;
}bool PixelIterator::CheckInTriangle(glm::ivec2 position)
{
    return CheckInTriangle(screenPosition[0], screenPosition[1], screenPosition[2], position);
}
bool PixelIterator::CheckInTriangle()
{
    return CheckInTriangle(screenPosition[0], screenPosition[1], screenPosition[2], glm::ivec2(x, y));
}

glm::dvec3 PixelIterator::GetBarycentricCoordinates()
{
    glm::ivec2 pos1 = screenPosition[0];
    glm::ivec2 pos2 = screenPosition[1];
    glm::ivec2 pos3 = screenPosition[2];
    double ratio1 = double(-(x - pos2.x) * (pos3.y - pos2.y) + (y - pos2.y) * (pos3.x - pos2.x))
        / double(-(pos1.x - pos2.x) * (pos3.y - pos2.y) + (pos1.y - pos2.y) * (pos3.x - pos2.x));
    ratio1 = std::clamp(ratio1, 0.0, 1.0);
    double ratio2 = double(-(x - pos3.x) * (pos1.y - pos3.y) + (y - pos3.y) * (pos1.x - pos3.x))
        / double(-(pos2.x - pos3.x) * (pos1.y - pos3.y) + (pos2.y - pos3.y) * (pos1.x - pos3.x));
    ratio2 = std::clamp(ratio2, 0.0, 1.0);
    double ratio3 = 1.0 - ratio1 - ratio2;
    ratio3 = std::clamp(ratio3, 0.0, 1.0);

    return glm::dvec3(ratio1, ratio2, ratio3);
}
glm::ivec2 PixelIterator::GetScreenPosition()
{
    return glm::ivec2(x, y);
}