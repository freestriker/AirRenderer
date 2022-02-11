#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"
class Transform: public Component
{
public:
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::mat4 modelMatrix;
	glm::mat4 worldMatrix;

	glm::mat4 NegativeTranslationMatrix();
	glm::mat4 NegativeRotationMatrix();
	glm::mat4 NegativeScaleMatrix();
	glm::mat4 TranslationMatrix();
	glm::mat4 RotationMatrix();
	glm::mat4 ScaleMatrix();

	void SetTranslation(glm::vec3 translation);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	void SetTranslationRotationScale(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
	void UpdateSelf(void* data);
	void UpdateGameObject(void* data);

	Transform();
	Transform(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
};

