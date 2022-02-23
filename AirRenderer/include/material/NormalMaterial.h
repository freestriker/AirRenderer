#pragma once
#include <include/material/Material.h>
#include <include/shader/NormalShader.h>
class NormalMaterial: public Material<NormalData, NormalShader>
{
public:
	NormalMaterial(std::function<void(NormalData&)> loadValue);
	MaterialBase* Clone();
};