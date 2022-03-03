#pragma once
#include <include/component/camera/Camera.h>
class OrthographicCamera: public Camera
{
public:
	float size;
	OrthographicCamera();
	glm::mat4 ProjectionMatrix() override;
	void ClipPlanes(glm::vec4* planes)override;
};