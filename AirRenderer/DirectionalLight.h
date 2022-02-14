#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	glm::vec3 IncidentLight(glm::vec3& position, glm::mat4& viewMatrix);
	glm::vec3 IncidentLight(glm::vec3& position);
	Color ColorIntensity(glm::vec3& incidentLight);
	DirectionalLight();
};

