#include <include/material/NormalMaterial.h>

NormalMaterial::NormalMaterial(std::function<void(NormalData&)> loadValue): Material(loadValue)
{

}

MaterialBase* NormalMaterial::Clone()
{
    return new NormalMaterial(*this);
}
