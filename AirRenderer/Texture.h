#pragma once
#include <string>
#include <FreeImage.h>
#include <glm/ext.hpp>
#include "Color.h"

class Texture
{
public:
	enum class InterpolationOption
	{
		NEAREST,
		BILINERA
	};
	enum class MipMapOption
	{
		NONE
	};
	enum class WrapOption
	{
		REPEAT,
		CLAMP,
		MIRROR
	};
	static const glm::fvec2 fzero;
	static const glm::fvec2 fhalf;
	static const glm::fvec2 fone;
	static const glm::ivec2 izero;
	static const glm::ivec2 ione;
	FIBITMAP* texture;
	InterpolationOption interpolationOption;
	WrapOption wrapOption;
	MipMapOption mipMapOption;
	std::string filePath;
	glm::ivec2 size;
	Color GetColor(glm::fvec2 pos);
	glm::fvec2 Wrap(glm::fvec2 uv);
	Color Interpolate(glm::fvec2& uv);
	void LoadTexture();
	Texture();
	Color Sample(glm::fvec2& uv);
	glm::vec3 SampleNormal(glm::fvec2& uv);
	Texture(std::string filaPath, InterpolationOption interpolationOption, WrapOption wrapOption, MipMapOption mipMapOption);
};

