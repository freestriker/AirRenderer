#pragma once
#include <include/component/camera/Camera.h>
class PerspectiveCamera : public Camera
{
public:
	float fovAngle;
	PerspectiveCamera();
	glm::mat4 ProjectionMatrix() override;
	void ClipPlanes(glm::vec4* planes)override;

};