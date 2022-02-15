#include "include/texture/Texture.h"
#include <algorithm>
#include <QDebug>

const glm::fvec2 Texture::fzero = glm::fvec2(0, 0);
const glm::fvec2 Texture::fhalf = glm::fvec2(0.5, 0.5);
const glm::fvec2 Texture::fone = glm::fvec2(1, 1);
const glm::ivec2 Texture::izero = glm::ivec2(0, 0);
const glm::ivec2 Texture::ione = glm::ivec2(1, 1);

Texture::Texture() :Texture("..\\Texture\\DefaultTexture.png", InterpolationOption::NEAREST, WrapOption::CLAMP, MipMapOption::NONE)
{
}

Texture::Texture(std::string filaPath, InterpolationOption interpolationOption, WrapOption wrapOption, MipMapOption mipMapOption)
{
    this->filePath = filaPath;
    this->interpolationOption = interpolationOption;
    this->wrapOption = wrapOption;
    this->mipMapOption = mipMapOption;
    this->texture = nullptr;

    LoadTexture();
}

void Texture::LoadTexture() 
{
    FreeImage_Initialise(TRUE);

    const char* filePath = this->filePath.c_str();
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filePath);
    if (fif == FREE_IMAGE_FORMAT::FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filePath);

    if ((fif != FREE_IMAGE_FORMAT::FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif))
    {
        texture = FreeImage_Load(fif, filePath);
    }
    else
    {
        throw "Can not read texture " + this->filePath + " failed.";
    }
    if (texture == nullptr)
    {
        throw "Read texture " + this->filePath + " failed.";
    }

    this->size = glm::ivec2(FreeImage_GetWidth(texture), FreeImage_GetHeight(texture));

    FreeImage_DeInitialise();
}

Color Texture::Sample(glm::fvec2& uv)
{
    glm::fvec2 pos = Wrap(uv);
    Color c = Interpolate(uv);
    return c;
}
glm::vec3 Texture::SampleNormal(glm::fvec2& uv)
{
    glm::fvec2 pos = Wrap(uv);
    Color c = Interpolate(uv);
    glm::vec3 normal = (glm::vec3(c.r, c.g, c.b) - glm::vec3(0.5, 0.5, 0.5)) * float(2);
    return normal;
}

glm::fvec2 Texture::Wrap(glm::fvec2 uv)
{
    glm::fvec2 result = uv;
    switch (this->wrapOption)
    {
        case WrapOption::REPEAT:
        {
            result = glm::fvec2(std::fmodf(uv.x, 1), std::fmodf(uv.y, 1));;
            break;
        }
        case WrapOption::MIRROR:
        {
            result = glm::abs(glm::fvec2(std::fmodf(uv.x, 2), std::fmodf(uv.y, 2)) - fone);
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
Color Texture::Interpolate(glm::fvec2& uv)
{
    glm::fvec2 fxy = glm::fvec2(uv.x * this->size.x, uv.y * this->size.y) - fhalf;
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
            fxy = glm::fvec2(std::fmodf(fxy.x, 1), std::fmodf(fxy.y, 1));
            glm::fvec2 scale = glm::fclamp(fxy, fzero, fone);
            Color floorColor = GetColor(glm::fvec2(x[0], y[0])) * (1.0f - scale[0]) + GetColor(glm::fvec2(x[1], y[0])) * scale[0];
            Color ceilColor = GetColor(glm::fvec2(x[0], y[1])) * (1.0f - scale[0]) + GetColor(glm::fvec2(x[1], y[1])) * scale[0];
            return floorColor * (1.0f - scale[1]) + ceilColor * scale[1];
        }
    }
    throw "";
}
Color Texture::GetColor(glm::fvec2 pos)
{
    pos = glm::clamp(pos, fzero, glm::fvec2(this->size));
    glm::ivec2 position = glm::clamp(glm::iround(pos), izero, this->size - ione);
    FIRGB16* bits1 = nullptr;
    FIRGBF* bits2 = nullptr;
    FIRGBA16* bits3 = nullptr;
    FIRGBAF* bits4 = nullptr;
    BYTE* bits5 = nullptr;
    FREE_IMAGE_TYPE t = FreeImage_GetImageType(texture);
    int i = 0;
    switch (t)
    {
        case FIT_BITMAP:
            //qDebug() << FreeImage_GetBPP(texture);
            //i = FreeImage_GetBPP(texture) * position.x;
            //bits5 = (BYTE*)FreeImage_GetBits(texture);
            RGBQUAD c = RGBQUAD();
            FreeImage_GetPixelColor(texture, position.x, position.y, &c);
            return Color(float(c.rgbRed) / 255, float(c.rgbGreen) / 255, float(c.rgbBlue) / 255, 1);
        case FIT_RGB16:
            bits1 = (FIRGB16*)FreeImage_GetScanLine(texture, position.y);
            return Color(float(bits1[position.x].red) / SHRT_MAX, float(bits1[position.x].green) / SHRT_MAX, float(bits1[position.x].blue) / SHRT_MAX, 1);
        case FIT_RGBF:
            bits2 = (FIRGBF*)FreeImage_GetScanLine(texture, position.y);
            return Color(bits2[position.x].red, bits2[position.x].green, bits2[position.x].blue, 1);
        case FIT_RGBA16:
            bits3 = (FIRGBA16*)FreeImage_GetScanLine(texture, position.y);
            return Color(float(bits3[position.x].red) / SHRT_MAX, float(bits3[position.x].green) / SHRT_MAX, float(bits3[position.x].blue) / SHRT_MAX, float(bits3[position.x].alpha) / SHRT_MAX);
        case FIT_RGBAF:
            bits4 = (FIRGBAF*)FreeImage_GetScanLine(texture, position.y);
            return Color(bits4[position.x].red, bits4[position.x].green, bits4[position.x].blue, bits4[position.x].alpha);
    }
    return Color();
}