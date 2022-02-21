#pragma once
#include <string>
#include <glm/ext.hpp>
#include "include/utils/Color.h"
#include <include/thread/LoadThread.h>

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
	static const glm::vec2 fzero;
	static const glm::vec2 fhalf;
	static const glm::vec2 fone;
	static const glm::ivec2 izero;
	static const glm::ivec2 ione;
	InterpolationOption interpolationOption;
	WrapOption wrapOption;
	MipMapOption mipMapOption;
	LoadThread::LoadCommand loadCommand;
	Color GetColor(glm::vec2 pos, void* texture, glm::ivec2& size);
	glm::vec2 Wrap(glm::vec2 uv);
	Color Interpolate(glm::vec2& uv, void* texture, glm::ivec2& size);
	Texture();
	Color Sample(glm::vec2& uv);
	glm::vec3 SampleNormal(glm::vec2& uv);
	Texture(std::string filaPath, InterpolationOption interpolationOption, WrapOption wrapOption, MipMapOption mipMapOption);
};

