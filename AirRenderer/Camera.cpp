#include "Camera.h"

glm::mat4 Camera::ProjectionMatrix()
{
    return glm::mat4(
        1.0 / (aspectRatio * size), 0, 0, 0,
        0, 1.0 / size, 0, 0,
        0, 0, -1.0 / (farFlat - nearFlat), 0,
        0, 0, -nearFlat / (farFlat - nearFlat), 1
        );
    //return glm::mat4(
    //    1 / (aspectRatio * size), 0, 0, 0,
    //    0, 1 / size, 0, 0,
    //    0, 0, -2 / (farFlat - nearFlat), 0,
    //    0, 0, -(farFlat + nearFlat) / (farFlat - nearFlat) + 0.5, 1
    //    );
}
glm::mat4 Camera::ObserveMatrix(Transform transform)
{
    return transform.NegativeRotationMatrix() * transform.NegativeTranslationMatrix();
}

Camera::Camera()
{
    this->nearFlat = 0;
    this->farFlat = 100;
    this->aspectRatio = 16.0 / 9.0;
    this->size = 9;
}
