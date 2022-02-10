#include "Material.h"
#include "Texture.h"
#include <QtCore/qcoreapplication.h>
Material::Material()
{
	std::string s = (QCoreApplication::applicationDirPath() + '/' + "../../Texture/Wall.png").toStdString();
	this->texture = new Texture(s, Texture::InterpolationOption::BILINERA, Texture::WrapOption::CLAMP, Texture::MipMapOption::NONE);
}