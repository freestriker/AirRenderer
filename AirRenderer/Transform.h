#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
class Transform
{
public:
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::mat4 NegativeTranslationMatrix();
	glm::mat4 NegativeRotationMatrix();
	glm::mat4 NegativeScaleMatrix();
	glm::mat4 TranslationMatrix();
	glm::mat4 RotationMatrix();
	glm::mat4 ScaleMatrix();
	Transform();
	Transform(glm::vec3 translation, glm::quat rotation, glm::vec3 scale);
};

