#include "include/component/transform/Transform.h"
#include "include/core_object/GameObject.h"
#include "include/utils/Log.h"
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

void Transform::SetTranslation(glm::vec3 translation)
{
    this->translation = translation;

    GameObject* parentGameObject = this->gameObject ? this->gameObject->parent : nullptr;
    UpdateSelf(parentGameObject);
    UpdateGameObject(parentGameObject);
}

void Transform::SetRotation(glm::quat rotation)
{
    this->rotation = rotation;

    GameObject* parentGameObject = this->gameObject ? this->gameObject->parent : nullptr;
    UpdateSelf(parentGameObject);
    UpdateGameObject(parentGameObject);
}

void Transform::SetScale(glm::vec3 scale)
{
    this->scale = scale;

    GameObject* parentGameObject = this->gameObject ? this->gameObject->parent : nullptr;
    UpdateSelf(parentGameObject);
    UpdateGameObject(parentGameObject);
}

void Transform::SetTranslationRotationScale(glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
    this->translation = translation;
    this->rotation = rotation;
    this->scale = scale;

    GameObject* parentGameObject = this->gameObject ? this->gameObject->parent : nullptr;
    UpdateSelf(parentGameObject);
    UpdateGameObject(parentGameObject);
}
void Transform::UpdateSelf(void* data)
{
    GameObject* parentGameObject = static_cast<GameObject*>(data);
    //Utils::LogMatrix("TranslationMatrix", this->TranslationMatrix());
    //Utils::LogMatrix("RotationMatrix", this->RotationMatrix());
    //Utils::LogMatrix("ScaleMatrix", this->ScaleMatrix());

    this->modelMatrix = TranslationMatrix() * RotationMatrix() * ScaleMatrix();
    this->worldMatrix = parentGameObject ? parentGameObject->transform.worldMatrix * this->modelMatrix : this->modelMatrix;
    //Utils::LogMatrix("ModelMatrix", this->modelMatrix);
    //Utils::LogMatrix("WorldMatrix", this->worldMatrix);
}
void Transform::UpdateGameObject(void* data)
{
    this->gameObject->UpdateSelfWithoutTransform(data);
    this->gameObject->CascadeUpdate(data);
}

Transform::Transform():Transform(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(1, 1, 1))
{

}

Transform::Transform(glm::vec3 translation, glm::quat rotation, glm::vec3 scale):Component("Transform")
{
    this->translation = translation;
    this->rotation = rotation;
    this->scale = scale;
    this->modelMatrix = TranslationMatrix() * RotationMatrix() * ScaleMatrix();
    this->worldMatrix = this->modelMatrix;
}