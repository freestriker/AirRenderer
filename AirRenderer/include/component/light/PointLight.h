#pragma once
#include "include/component/light/Light.h"
class PointLight: public Light
{
public:
	float emitterRadius;
	float lightRadius;
	glm::vec3 IncidentLight(glm::vec3& position, glm::mat4& viewMatrix);
	glm::vec3 IncidentLight(glm::vec3& position);
	Color ColorIntensity(glm::vec3& incidentLight);
	Color AmbintColorIntensity();

	PointLight();
	virtual ~PointLight();
	Light* Clone();
};

