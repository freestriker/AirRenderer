#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera:public GameObject
{
public:
	float near;
	float far;
	float aspectRatio;
	float size;
	glm::mat4 ProjectionMatrix(Transform transform);
	glm::mat4 Camera::ObserveMatrix(Transform transform);
};

