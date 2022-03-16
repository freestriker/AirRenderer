#include <include/material/Material.h>

Material* Material::Clone()
{
	return new Material(value->Clone(), shader->Clone());
}

void Material::FillData()
{
	shader->FillData(value);
}

Material::~Material()
{
	typeName = "DestoriedMaterial";
	delete value;
	delete shader;
}

Material::Material(ShaderData* value, Shader* shader) : Object("Material")
{
	this->value = value;
	this->shader = shader;
}