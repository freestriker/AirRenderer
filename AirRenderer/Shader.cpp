#include "Shader.h"
#include "Texture.h"

void Shader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, Material& material)
{
	vertexOutContext.position = vertexInContext.mvpMatrix * vertexInContext.position;
	vertexOutContext.texcoord1 = vertexInContext.texcoord1;
	vertexOutContext.color = vertexInContext.color;
}

void Shader::PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, Material& material)
{
	Color c = static_cast<Texture*>(material.texture)->Sample(vertexInContext.texcoord1);
	//c = c * vertexInContext.color;
	vertexOutContext.color = c;
}
