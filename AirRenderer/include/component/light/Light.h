#pragma once
#include "include/component/Component.h"
#include "include/utils/Color.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
class Light: public Component
{
public:
	std::string lightTypeName;
	Color baseColor;
	float intensity;
	glm::mat4 worldMatrix;
	virtual glm::vec3 IncidentLight(glm::vec3& position, glm::mat4& viewMatrix) = 0;
	virtual glm::vec3 IncidentLight(glm::vec3& position) = 0;
	virtual Color ColorIntensity(glm::vec3& incidentLight) = 0;
	virtual Color AmbintColorIntensity();
	virtual Light* Clone() = 0;
	Light(std::string lightTypeName);
	void UpdateSelf(void* data)override;
};

