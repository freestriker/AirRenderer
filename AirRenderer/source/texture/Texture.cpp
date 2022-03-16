#include "include/texture/Texture.h"
#include <algorithm>
#include <QDebug>
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>
#include "include/utils/Color.h"
const glm::vec2 Texture::fzero = glm::vec2(0, 0);
const glm::vec2 Texture::fhalf = glm::vec2(0.5, 0.5);
const glm::vec2 Texture::fone = glm::vec2(1, 1);
const glm::ivec2 Texture::izero = glm::ivec2(0, 0);
const glm::ivec2 Texture::ione = glm::ivec2(1, 1);

Texture::Texture():Object("Texture"), loadCommand()
{
    this->bitmap = nullptr;
}
Texture::Texture(std::string filePath, InterpolationOption interpolationOption, WrapOption wrapOption, MipMapOption mipMapOption) : Object("Texture")
{
    this->interpolationOption = interpolationOption;
    this->wrapOption = wrapOption;
    this->mipMapOption = mipMapOption;
    this->loadCommand = global.loadThread->Load(filePath, LoadThread::ProcessOptions::TEXTURE);
    this->bitmap = nullptr;
}

Texture::~Texture()
{
    global.loadThread->Unload(loadCommand);
    typeName = "DestoriedTexture";
}

Texture* Texture::Clone()
{
    return new Texture(loadCommand.path, interpolationOption, wrapOption, mipMapOption);
}

Color Texture::Sample(glm::vec2& uv)
{
    
    if (this->bitmap == nullptr)
    {
        this->bitmap = global.loadThread->GetResource<FIBITMAP>(loadCommand);
        this->size = glm::vec2(FreeImage_GetWidth(this->bitmap), FreeImage_GetHeight(this->bitmap));
    }
    glm::vec2 pos = Wrap(uv);
    Color c = Interpolate(uv);
    return c;
}
FIBITMAP* Texture::GetTexture()
{
    return loadCommand.valid ? global.loadThread->GetResource<FIBITMAP>(loadCommand) : nullptr;
}



glm::vec3 Texture::SampleNormal(glm::vec2& uv)
{
    Color c = Sample(uv);
    glm::vec3 normal = (glm::vec3(c.r, c.g, c.b) - glm::vec3(0.5, 0.5, 0.5)) * float(2);
    return normal;
}

glm::vec2 Texture::Wrap(glm::vec2 uv)
{
    glm::vec2 result = uv;
    switch (this->wrapOption)
    {
        case WrapOption::REPEAT:
        {
            result = glm::vec2(std::fmodf(uv.x, 1), std::fmodf(uv.y, 1));;
            break;
        }
        case WrapOption::MIRROR:
        {
            result = glm::abs(glm::vec2(std::fmodf(uv.x, 2), std::fmodf(uv.y, 2)) - fone);
            break;
        }
        case WrapOption::CLAMP:
        {
            break;
        }
    }
    result = glm::fclamp(result, fzero, fone);
    return result;
}
Color Texture::Interpolate(glm::vec2& uv)
{
    glm::vec2 fxy = glm::vec2(uv.x * size.x, uv.y * size.y) - fhalf;
    switch (this->interpolationOption)
    {
        case InterpolationOption::NEAREST:
        {
            return GetColor(fxy);
        }
        case InterpolationOption::BILINERA:
        {
            float x[2]{std::floor(fxy.x), std::ceil(fxy.x)};
            float y[2]{std::floor(fxy.y), std::ceil(fxy.y)};
            fxy = glm::vec2(std::fmodf(fxy.x, 1), std::fmodf(fxy.y, 1));
            glm::vec2 scale = glm::fclamp(fxy, fzero, fone);
            Color floorColor = GetColor(glm::vec2(x[0], y[0])) * (1.0f - scale[0]) + GetColor(glm::vec2(x[1], y[0])) * scale[0];
            Color ceilColor = GetColor(glm::vec2(x[0], y[1])) * (1.0f - scale[0]) + GetColor(glm::vec2(x[1], y[1])) * scale[0];
            return floorColor * (1.0f - scale[1]) + ceilColor * scale[1];
        }
    }
    throw "";
}
Color Texture::GetColor(glm::vec2 pos)
{
    pos = glm::clamp(pos, fzero, glm::vec2(size));
    glm::ivec2 position = glm::clamp(glm::iround(pos), izero, size - ione);

    int bytespp = FreeImage_GetLine(bitmap) / size.x;
    BYTE* bits = FreeImage_GetScanLine(bitmap, position.y);
    bits += bytespp * position.x;
    return Color(float(bits[FI_RGBA_RED]) / 255, float(bits[FI_RGBA_GREEN]) / 255, float(bits[FI_RGBA_BLUE]) / 255, bits[FI_RGBA_ALPHA]);

}