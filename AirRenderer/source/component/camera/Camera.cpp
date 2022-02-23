#include "include/component/camera/Camera.h"
#include <include/core_object/GameObject.h>

void Camera::UpdateSelf(void* data)
{
}
glm::mat4 Camera::WorldMatrix()
{
    return gameObject->transform.worldMatrix;
}

Camera::Camera(std::string cameraType) :Component("Camera")
{
    this->cameraType = cameraType;
    this->needPerspectiveCorrection = false;
}
