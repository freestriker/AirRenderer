#pragma once
#include <glm/mat4x4.hpp>
#include <include/utils/PrimitiveCliper.h>
class CameraContext
{
public:
	bool needPerspectiveCorrection;
	glm::mat4 projectionMatrix;
	glm::mat4 worldMatrix;
	float nearFlat;
	float farFlat;
	float aspectRatio;
	PrimitiveCliper primitiveCliper;
};