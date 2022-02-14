#pragma once
class Material
{
public:
	void* texture;
	void* normalMap;
	float ambientReflectance;
	float diffuseReflectance;
	float specularReflectance;
	float convergenceExponent;
	Material();
};

