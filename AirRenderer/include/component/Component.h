#pragma once
#include "include/utils/Object.h"
class GameObject;
class Component: public Object
{
public:
	GameObject* gameObject;
	Component(std::string typeName);
	virtual void UpdateSelf(void* data);
};

