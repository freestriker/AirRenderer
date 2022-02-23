#pragma once
#include <glm/mat4x4.hpp>
class CameraContext
{
public:
	bool needPerspectiveCorrection;
	glm::mat4 projectionMatrix;
	glm::mat4 worldMatrix;
};