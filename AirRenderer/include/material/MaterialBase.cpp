#include "MaterialBase.h"

MaterialBase::~MaterialBase()
{
	typeName = "DestoriedMaterial";
}

MaterialBase::MaterialBase(): Object("Material")
{
}
