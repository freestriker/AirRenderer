#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ChildBrotherTree.h"
class Transform: public ChildBrotherTree<Transform>
{
public:
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::mat4 TranslationMatrix();
	glm::mat4 RotationMatrix();
	glm::mat4 ScaleMatrix();
};

