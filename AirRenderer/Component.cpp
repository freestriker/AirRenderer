#include "Component.h"

Component::Component(std::string typeName): Object(typeName)
{
	gameObject = nullptr;
	
}

void Component::UpdateSelf(void* data)
{
}
