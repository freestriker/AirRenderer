#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Transform.h"

class Camera
{
public:
	float aspectRatio;
	float size;
	float nearFlat;
	float farFlat;
	glm::mat4 ProjectionMatrix();
	glm::mat4 Camera::ObserveMatrix(Transform transform);
	Camera();
};

