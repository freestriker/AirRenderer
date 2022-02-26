#include <include/shader/ShaderBase.h>
ShaderBase::ShaderBase()
{
	for (int i = 0; i < MAX_SHADER_PASS_COUNT; i++)
	{
		activeTable[i] = false;
		shaderPasses[i] = ShaderPass();
	}
}

ShaderPass::ShaderPass()
{
	cullOption = CullOption::CULL_BACK;
	vertexShading = nullptr;
	pixelShading = nullptr;
	geometryShading = nullptr;
}
