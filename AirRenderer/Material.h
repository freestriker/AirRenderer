#pragma once
class Material
{
public:
	void* texture;
	float ambientReflectance;
	float diffuseReflectance;
	float specularReflectance;
	float convergenceExponent;
	Material();
};

