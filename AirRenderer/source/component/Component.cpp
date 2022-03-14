#include "include/component/Component.h"

Component::Component(std::string typeName): Object(typeName), CrossLinkedNode()
{
	gameObject = nullptr;	
}

Component::~Component()
{
	gameObject = nullptr;
}

void Component::UpdateSelf(void* data)
{
}