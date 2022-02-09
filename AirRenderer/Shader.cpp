#include "Shader.h"

void Shader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext)
{
	vertexOutContext.position = vertexInContext.mvpMatrix * vertexInContext.position;
	vertexOutContext.color = vertexInContext.color;
}

void Shader::PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext)
{
	vertexOutContext.color = vertexInContext.color;
}
