#include "Transform.h"

glm::mat4 Transform::TranslationMatrix()
{
    return glm::mat4(glm::vec4(0, 0, 0, scale.x), glm::vec4(0, 0, 0, scale.y), glm::vec4(0, 0, 0, scale.z), glm::vec4(0, 0, 0, 1));
}

glm::mat4 Transform::RotationMatrix()
{
    return glm::mat4_cast(rotation);
}

glm::mat4 Transform::ScaleMatrix()
{
    return glm::mat4(glm::vec4(scale.x, 0, 0, 0), glm::vec4(0, scale.y, 0, 0), glm::vec4(0, 0, scale.z, 0), glm::vec4(0, 0, 0, 1));
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
