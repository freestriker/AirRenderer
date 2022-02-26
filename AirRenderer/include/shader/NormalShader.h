#pragma once
#include <include/shader/NormalData.h>
#include <include/shader/Shader.h>
class NormalShader: public Shader<NormalData>
{
public:
	void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void GeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);
	NormalShader();
};