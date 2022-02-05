#include "Transform.h"

glm::mat4 Transform::NegativeTranslationMatrix()
{
    return glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -translation.x, -translation.y, -translation.z, 1
    );
}

glm::mat4 Transform::NegativeRotationMatrix()
{
    return glm::mat4_cast(glm::quat(rotation.w, -rotation.x, -rotation.y, -rotation.z));
}

glm::mat4 Transform::NegativeScaleMatrix()
{
    return glm::mat4(
        -scale.x, 0, 0, 0,
        0, -scale.y, 0, 0,
        0, 0, -scale.z, 0,
        0, 0, 0, 1
    );
}
glm::mat4 Transform::TranslationMatrix()
{
    return glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        translation.x, translation.y, translation.z, 1
    );
}

glm::mat4 Transform::RotationMatrix()
{
    return glm::mat4_cast(rotation);
}

glm::mat4 Transform::ScaleMatrix()
{
    return glm::mat4(
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1
    );
}

Transform::Transform():Transform(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1))
{

}

Transform::Transform(glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
    this->translation = translation;
    this->rotation = rotation;
    this->scale = scale;
}
