#pragma once
#include <include/texture/Texture.h>
class NormalData
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
};