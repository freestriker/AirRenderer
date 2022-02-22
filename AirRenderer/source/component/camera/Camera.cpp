#include "include/component/camera/Camera.h"

void Camera::UpdateSelf(void* data)
{
}

Camera::Camera(std::string cameraType) :Component("Camera")
{
    this->cameraType = cameraType;
    this->needPerspectiveCorrection = false;
}
