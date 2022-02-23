#include "include/component/light/Light.h"
#include <include/core_object/GameObject.h>
Color Light::AmbintColorIntensity()
{
	return this->baseColor * intensity;
}

Light::Light(std::string lightTypeName):Component("Light")
{
	this->lightTypeName = lightTypeName;
	this->baseColor = Color(1, 235.0 / 255, 205.0 / 255, 1);
	this->intensity = 0.8;
}

void Light::UpdateSelf(void* data)
{
	this->worldMatrix = this->gameObject->transform.worldMatrix;
}
