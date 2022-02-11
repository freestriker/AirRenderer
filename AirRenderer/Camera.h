#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"

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

