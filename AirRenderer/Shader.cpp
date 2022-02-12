#include "Shader.h"
#include "Texture.h"

void Shader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext)
{
	vertexOutContext.position = matrixContext->wvpMatrix * vertexInContext.position;
	vertexOutContext.texcoord1 = vertexInContext.texcoord1;
	vertexOutContext.normal = glm::mat3(matrixContext->wv_tiMatrix) * vertexInContext.normal;
	if (vertexInContext.vertexIndex % 3 == 0)
	{
		vertexOutContext.color = Color::red;
	}
	else if (vertexInContext.vertexIndex % 3 == 1)
	{
		vertexOutContext.color = Color::green;
	}
	else if(vertexInContext.vertexIndex % 3 == 2)
	{
		vertexOutContext.color = Color::blue;
	}
	else
	{
		vertexOutContext.color = vertexInContext.color;
	}
}

void Shader::PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext)
{
	Color c = vertexInContext.color; 
	c = c * static_cast<Texture*>(material.texture)->Sample(vertexInContext.texcoord1);
	vertexOutContext.color = c;
}
