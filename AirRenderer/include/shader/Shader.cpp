#include "Shader.h"

ShaderPass::ShaderPass()
{
}

ShaderPass::~ShaderPass()
{

}

glm::vec3 Shader::SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2)
{
	glm::normalize(v1);
	v2 = glm::normalize(v2 - glm::dot(v1, v2) * v1);
	return glm::normalize(glm::cross(v1, v2));
}

Shader::Shader() :Object("Shader")
{
	value = nullptr;
	shaderPasses = std::vector< ShaderPass>();
}
Shader::~Shader()
{
	value = nullptr;
	shaderPasses.clear();
	typeName = "DestoriedShader";
}
void Shader::DefaultVertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	vertexOutContext.data[RegisterIndex::POSITION] = matrixContext->wvpMatrix * vertexInContext.data[RegisterIndex::POSITION];

	vertexOutContext.data[RegisterIndex::COLOR] = vertexInContext.data[RegisterIndex::COLOR];
}
bool Shader::DefaultPixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	pixelOutContext.color = Color(pixelInContext.data[RegisterIndex::COLOR]);
	pixelOutContext.depth = pixelInContext.depth;
	return false;
}
void Shader::DefaultGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)
{
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveInContext);
}

void Shader::FillData(ShaderData* value)
{
	this->value = value;
}

