#pragma once
#include <include/component/camera/Camera.h>
class OrthographicCamera: public Camera
{
public:
	float aspectRatio;
	float size;
	float nearFlat;
	float farFlat;
	OrthographicCamera();
	glm::mat4 ProjectionMatrix() override;
};