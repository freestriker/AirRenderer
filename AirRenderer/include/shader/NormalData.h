#pragma once
#include <include/texture/Texture.h>
#include <include/shader/ShaderData.h>
class NormalData: public ShaderData
{
public:
	Texture* texture;
	Texture* normalMap;
	float ambientReflectance;
	float diffuseReflectance;
	float specularReflectance;
	float convergenceExponent;
	~NormalData()
	{
		delete texture;
		delete normalMap;
	}
	NormalData(): ShaderData()
	{
		ambientReflectance = 0.2;
		convergenceExponent = 19;
		diffuseReflectance = 1;
		specularReflectance = 4;
		texture = new Texture("..\\Resources\\Texture\\Wall.png", Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
		normalMap = new Texture("..\\Resources\\Texture\\Wall_Normal.png", Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
	}
	ShaderData* Clone()override
	{
		return new NormalData();
	}
};