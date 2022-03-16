#pragma once
#include <include/shader/Shader.h>
#include <include/shader/ShaderData.h>
class Material:public Object
{
public:
	ShaderData* value;
	Shader* shader;
	Material* Clone();
	void FillData();
	~Material();
	Material(ShaderData* value, Shader* shader);
};
