#include "include/material/Material.h"
#include "include/texture/Texture.h"
#include <QtCore/qcoreapplication.h>
Material::Material()
{
	this->ambientReflectance = 0.2;
	this->diffuseReflectance = 1;
	this->specularReflectance = 4;
	this->convergenceExponent = 19;
	std::string s = (QCoreApplication::applicationDirPath() + '/' + "../../Resources/Texture/Wall.png").toStdString();
	this->texture = new Texture(s, Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
	s = (QCoreApplication::applicationDirPath() + '/' + "../../Resources/Texture/Wall_Normal.png").toStdString();
	this->normalMap = new Texture(s, Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
}