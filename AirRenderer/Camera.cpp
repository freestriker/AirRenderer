#include "Camera.h"

glm::mat4 Camera::ProjectionMatrix(Transform transform)
{
    return glm::mat4(
        1 / (aspectRatio * size), 0, 0, 0,
        0, 1 / size, 0, 0,
        0, 0, -2 / (farFlat - nearFlat), 0,
        0, 0, -(farFlat + nearFlat) / (farFlat - nearFlat) + 0.5, 1
        );
}
glm::mat4 Camera::ObserveMatrix(Transform transform)
{
    return transform.NegativeRotationMatrix() * transform.NegativeTranslationMatrix();
}

Camera::Camera()
{
    this->nearFlat = 5;
    this->farFlat = 2000;
    this->aspectRatio = 16 / 9;
    this->size = 60;
}
