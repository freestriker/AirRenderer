#include "include/utils/Object.h"

void Object::UpdateSelf(void* data)
{
}

Object::Object()
{
	active = true;
	typeName = "Unknown";
}
Object::~Object()
{
	active = false;
	typeName = "Destoried";
}
Object::Object(std::string name)
{
	active = true;
	typeName = name;
}
