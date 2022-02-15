#pragma once
#include <glm/mat4x4.hpp>
#include "include/component/Component.h"

class Camera: public Component
{
public:
	float aspectRatio;
	float size;
	float nearFlat;
	float farFlat;
	glm::mat4 ProjectionMatrix();
	Camera();
	void UpdateSelf(void* data);
};

