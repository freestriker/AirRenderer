#pragma once
#include <glm/mat4x4.hpp>
#include "include/component/Component.h"

class Camera: public Component
{
public:
	std::string cameraType;
	bool needPerspectiveCorrection;
	virtual glm::mat4 ProjectionMatrix() = 0;
	void UpdateSelf(void* data);
	glm::mat4 WorldMatrix();
	Camera(std::string cameraType);
};

