#pragma once
#include <include/utils/Object.h>
class ShaderData: public Object
{
public:
	virtual ShaderData* Clone() = 0;
	ShaderData() : Object("ShaderData")
	{

	}
	virtual ~ShaderData()
	{
		typeName = "DestoriedShaderData";
	}

};