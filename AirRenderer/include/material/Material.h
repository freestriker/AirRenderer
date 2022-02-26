#pragma once
#include <functional>
#include <include/material/MaterialBase.h>
template<typename TValue, typename TShader/*, typename = std::enable_if<std::is_base_of<Shader<TValue>, TShader>::value>::type*/>
class Material:public MaterialBase
{
public:
	TValue value;
	TShader shader;
	Material(std::function<void(TValue&)> loadValue);
	ShaderBase* Shader() override;
	virtual MaterialBase* Clone()override = 0;

	Material();
};

template<typename TValue, typename TShader>
Material<TValue, TShader>::Material(std::function<void(TValue&)> loadValue)
{
	loadValue(value);
}

template<typename TValue, typename TShader>
ShaderBase* Material<TValue, TShader>::Shader()
{
	ShaderBase* sb = static_cast<ShaderBase*>(new TShader());
	sb->FillData(&value);
	return sb;
}

