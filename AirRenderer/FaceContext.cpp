#include "FaceContext.h"
#include "PixelIterator.h"

glm::ivec4 FaceContext::GetFaceScreenSize()
{
    int minX = std::min(screenPosition[0].x, std::min(screenPosition[1].x, screenPosition[2].x));
    int minY = std::min(screenPosition[0].y, std::min(screenPosition[1].y, screenPosition[2].y));
    int maxX = std::max(screenPosition[0].x, std::max(screenPosition[1].x, screenPosition[2].x));
    int maxY = std::max(screenPosition[0].y, std::max(screenPosition[1].y, screenPosition[2].y));
    return glm::ivec4(minX, minY, maxX, maxY);
}

PixelIterator FaceContext::GetStartPixelIterator()
{
    return PixelIterator(this, true);
}

PixelIterator FaceContext::GetEndPixelIterator()
{
    return PixelIterator(this, false);
}