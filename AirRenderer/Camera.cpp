#include "Camera.h"

glm::mat4 Camera::ProjectionMatrix(Transform transform)
{
    return glm::mat4(
        1 / (aspectRatio * size), 0, 0, 0,
        0, 1 / size, 0, 0,
        0, 0, -2 / (far - near), 0,
        0, 0, -(far + near) / (far - near) + 0.5, 1
        );
}
glm::mat4 Camera::ObserveMatrix(Transform transform)
{
    return transform.NegativeRotationMatrix() * transform.NegativeTranslationMatrix();
}
