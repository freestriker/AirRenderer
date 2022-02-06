#include "GameObject.h"

GameObject::GameObject()
{
	this->transform = Transform();
	this->meshRenderer = nullptr;
	this->camera = nullptr;
}