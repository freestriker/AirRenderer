#pragma once
#include "include/context/VertexInContext.h"
#include "include/context/VertexOutContext.h"
#include "include/context/PixelInContext.h"
#include "include/context/PixelOutContext.h"

#include "include/context/MatrixContext.h"
#include "include/context/LightContext.h"
#include <include/utils/RegisterIndex.h>
#include <include/shader/ShaderOptions.h>
#include <include/context/PrimitiveContext.h>
#include <functional>

class ShaderPass
{
public:
	std::string passName;
	ShaderOption shaderOption;
	std::function<void(VertexInContext&, VertexOutContext&, MatrixContext*, LightContext*)> vertexShading;
	std::function<bool(PixelInContext&, PixelOutContext&, MatrixContext*, LightContext*)> pixelShading;
	std::function<void(PrimitiveContext&, PrimitiveOutContextBuilder&, MatrixContext*, LightContext*)> geometryShading;

	ShaderPass();
};
class ShaderBase
{
public:
	virtual void FillData(void* data) = 0;
	std::vector< ShaderPass> shaderPasses;

	ShaderBase();
};
