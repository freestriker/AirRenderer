#pragma once
#include "VertexInContext.h"
#include "VertexOutContext.h"
#include "PixelInContext.h"
#include "PixelOutContext.h"
class Shader
{
public:
	void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext);
	void PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext);
};

