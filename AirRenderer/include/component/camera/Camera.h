#pragma once
#include <glm/mat4x4.hpp>
#include "include/component/Component.h"

class Camera: public Component
{
public:
	float nearFlat;
	float farFlat;
	float aspectRatio;
	std::string cameraType;
	bool needPerspectiveCorrection;
	virtual glm::mat4 ProjectionMatrix() = 0;
	virtual void ClipPlanes(glm::vec4* planes) = 0;
	virtual ~Camera();
	void UpdateSelf(void* data);
	glm::mat4 WorldMatrix();
	Camera(std::string cameraType);
};

