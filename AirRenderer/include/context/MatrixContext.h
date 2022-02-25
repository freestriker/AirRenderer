#pragma once
#include <glm/mat4x4.hpp>
class MatrixContext
{
public:
	glm::mat4 worldMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 rasterizationMatrix;
	glm::mat4 wvpMatrix;
	glm::mat4 wvMatrix;
	glm::mat4 wv_itMatrix;
	glm::mat4 w_itMatrix;
	glm::mat4 vpMatrix;
};

