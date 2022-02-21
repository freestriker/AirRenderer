#pragma once
#include <include/component/camera/Camera.h>
class PerspectiveCamera : public Camera
{
public:
	float aspectRatio;
	float fovAngle;
	float nearFlat;
	float farFlat;
	PerspectiveCamera();
	glm::mat4 ProjectionMatrix() override;
};