#pragma once
#include <include/shader/NormalData.h>
#include <include/shader/Shader.h>
class NormalShader: public Shader<NormalData>
{
public:
	void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void PixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void GeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);
	void WireframePixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void WireframeGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);

	void NormalPixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext);

	void NormalGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);

	NormalShader();
};