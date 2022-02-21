#pragma once
#include <glm/mat4x4.hpp>
#include "include/component/Component.h"

class Camera: public Component
{
public:
	std::string cameraType;
	virtual glm::mat4 ProjectionMatrix() = 0;
	void UpdateSelf(void* data);
	Camera(std::string cameraType);
};

