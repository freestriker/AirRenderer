#pragma once
#include <include/shader/ShaderBase.h>
class MaterialBase
{
public:
	virtual ShaderBase* Shader() = 0;
	virtual MaterialBase* Clone() = 0;
};
