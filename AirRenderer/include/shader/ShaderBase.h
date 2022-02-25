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
class ShaderBase
{
public:
	CullOption cullOption;
	virtual void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext) = 0;
	virtual void PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext) = 0;
	virtual void GeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext) = 0;
	virtual void FillData(void* data) = 0;
	ShaderBase();
};
