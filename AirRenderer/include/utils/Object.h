#pragma once
#include <string>
class Object
{
public:
	bool active;
	std::string typeName;
	virtual void UpdateSelf(void* data);
	Object();
	Object(std::string name);
};

