#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Color.h"
class Light: public Component
{
public:
	std::string lightTypeName;
	Color baseColor;
	float intensity;
	virtual glm::vec3 IncidentLight(glm::vec3& position, glm::mat4& viewMatrix) = 0;
	virtual glm::vec3 IncidentLight(glm::vec3& position) = 0;
	virtual Color ColorIntensity(glm::vec3& incidentLight) = 0;
	virtual Color AmbintColorIntensity();
	Light(std::string lightTypeName);
};

