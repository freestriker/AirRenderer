#include <include/material/NormalMaterial.h>

NormalMaterial::NormalMaterial(std::function<void(NormalData&)> loadValue): Material(loadValue)
{

}
NormalMaterial::~NormalMaterial()
{

}

MaterialBase* NormalMaterial::Clone()
{
    return new NormalMaterial(initer);
}
