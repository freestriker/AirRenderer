#pragma once
#include <include/shader/ShaderBase.h>
class MaterialBase:public Object
{
public:
	virtual ShaderBase* Shader() = 0;
	virtual MaterialBase* Clone() = 0;
	virtual ~MaterialBase();
	MaterialBase();
};
