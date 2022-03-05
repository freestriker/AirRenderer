#include <include/shader/ShaderBase.h>
ShaderBase::ShaderBase()
{
	shaderPasses = std::vector<ShaderPass>();
}

ShaderPass::ShaderPass()
{
	shaderOption = ShaderOption();
	passName = "";
	vertexShading = nullptr;
	pixelShading = nullptr;
	geometryShading = nullptr;
}
