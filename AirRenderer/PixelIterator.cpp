#include "PixelIterator.h"
#include "FaceContext.h"
#include <algorithm>

PixelIterator::PixelIterator(FaceContext* faceContext, bool start)
{
    this->faceContext = faceContext;
    this->size = faceContext->GetFaceScreenSize();
    if (start)
    {
        this->x = this->size.x;
        this->y = this->size.y;
    }
    else
    {
        this->x = this->size.z + 1;
        this->y = this->size.w + 1;
    }
}
PixelIterator& PixelIterator::operator++() {
    if (++x > size.z)
    {
        if (++y > size.w)
        {

        }
        else
        {
            this->x = this->size.x;
        }
    }
    return *this;
}
PixelIterator PixelIterator::operator++(int) {
    if (++x > size.z)
    {
        if (++y > size.w)
        {

        }
        else
        {
            this->x = this->size.x;
        }
    }
    return *this;
}
bool PixelIterator::operator==(PixelIterator other){ return x == other.x && y == other.y; }
bool PixelIterator::operator!=(PixelIterator other){ return !(x == other.x && y == other.y); }

PixelContext PixelIterator::operator*()
{
    PixelContext pixelContext = PixelContext();
    
    glm::ivec2 pos1 = faceContext->screenPosition[0];
    glm::ivec2 pos2 = faceContext->screenPosition[1];
    glm::ivec2 pos3 = faceContext->screenPosition[2];
    double ratio1 = double(-(x - pos2.x) * (pos3.y - pos2.y) + (y - pos2.y) * (pos3.x - pos2.x))
        / double(-(pos1.x - pos2.x) * (pos3.y - pos2.y) + (pos1.y - pos2.y) * (pos3.x - pos2.x));
    double ratio2 = double(-(x - pos3.x) * (pos1.y - pos3.y) + (y - pos3.y) * (pos1.x - pos3.x))
        / double(-(pos2.x - pos3.x) * (pos1.y - pos3.y) + (pos2.y - pos3.y) * (pos1.x - pos3.x));
    double ratio3 = 1.0 - ratio1 - ratio2;
    pixelContext.screenPosition = glm::ivec2(x, y);
    pixelContext.position = faceContext->vertexContext[0].position * float(ratio1) + faceContext->vertexContext[1].position * float(ratio2) + faceContext->vertexContext[2].position * float(ratio3);
    pixelContext.color = faceContext->vertexContext[0].color * ratio1 + faceContext->vertexContext[1].color * ratio2 + faceContext->vertexContext[2].color * ratio3;
    pixelContext.w = faceContext->w[0] * ratio1 + faceContext->w[1] * ratio2 + faceContext->w[2] * ratio3;
    pixelContext.z = faceContext->z[0] * ratio1 + faceContext->z[1] * ratio2 + faceContext->z[2] * ratio3;

    //float ratio1 = float(x - size.x) / float(size.z - size.x);
    //ratio1 = std::clamp(ratio1, float(0), float(1));
    //float ratio2 = float(y - size.y) / float(size.w - size.y);
    //ratio2 = std::clamp(ratio2, float(0), float(1));
    //float ratio3 = 1.0 - ratio1 - ratio2;
    //if (ratio3 >= float(0))
    //{
    //    ratio3 = std::clamp(ratio3, float(0), float(1));
    //    pixelContext.screenPosition = glm::fvec2(faceContext->screenPosition[0]) * ratio1 + glm::fvec2(faceContext->screenPosition[1]) * ratio2 + glm::fvec2(faceContext->screenPosition[2]) * ratio3;
    //    pixelContext.position = faceContext->vertexContext[0].position * ratio1 + faceContext->vertexContext[1].position * ratio2 + faceContext->vertexContext[2].position * ratio3;
    //    pixelContext.color = faceContext->vertexContext[0].color * ratio1 + faceContext->vertexContext[1].color * ratio2 + faceContext->vertexContext[2].color * ratio3;
    //    pixelContext.w = faceContext->w[0] * ratio1 + faceContext->w[1] * ratio2 + faceContext->w[2] * ratio3;
    //    pixelContext.z = faceContext->z[0] * ratio1 + faceContext->z[1] * ratio2 + faceContext->z[2] * ratio3;
    //}



    return pixelContext;
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
}bool PixelIterator::CheckInTriangle()
{
    return CheckInTriangle(faceContext->screenPosition[0], faceContext->screenPosition[1], faceContext->screenPosition[2], glm::ivec2(x, y));
}