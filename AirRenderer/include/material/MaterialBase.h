#pragma once
#include <include/shader/ShaderBase.h>
class MaterialBase
{
public:
	virtual ShaderBase* Shader() = 0;
};
