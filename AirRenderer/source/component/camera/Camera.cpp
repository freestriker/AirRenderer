#include "include/component/camera/Camera.h"

glm::mat4 Camera::ProjectionMatrix()
{
    return glm::mat4(
        1.0 / (aspectRatio * size), 0, 0, 0,
        0, 1.0 / size, 0, 0,
        0, 0, -1.0 / (farFlat - nearFlat), 0,
        0, 0, -nearFlat / (farFlat - nearFlat), 1
        );
}

Camera::Camera():Component("Camera")
{
    this->nearFlat = 0;
    this->farFlat = 100;
    this->aspectRatio = 16.0 / 9.0;
    this->size = 9;
}

void Camera::UpdateSelf(void* data)
{

}
