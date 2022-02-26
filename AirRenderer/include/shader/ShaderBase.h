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
class ShaderPass
{
public:
	typedef void (ShaderPass::*VertexShading)(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	typedef void (ShaderPass::*PixelShading)(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	typedef void (ShaderPass::*GeometryShading)(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);
	CullOption cullOption;
	VertexShading vertexShading;
	PixelShading pixelShading;
	GeometryShading geometryShading;

	ShaderPass();
};
class ShaderBase
{
public:
	virtual void FillData(void* data) = 0;
	ShaderPass shaderPasses[4];
	bool activeTable[4];

	ShaderBase();
};
