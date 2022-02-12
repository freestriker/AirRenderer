#pragma once
#include "Light.h"
class PointLight: public Light
{
public:
	float emitterRadius;
	float lightRadius;
	glm::vec3 IncidentLight(glm::vec3& position, glm::mat4& viewMatrix);
	Color ColorIntensity(glm::vec3& incidentLight);
	Color AmbintColorIntensity();

	PointLight();
};

