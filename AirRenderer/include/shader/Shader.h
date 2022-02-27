#pragma once
#include <include/shader/ShaderBase.h>

template<typename TValue>
class Shader:public ShaderBase
{
public:
	TValue* value;
	static glm::vec3 SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2);
	void FillData(void* data)override;
	Shader();
	void DefaultVertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	bool DefaultPixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void DefaultGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);
};

template<typename TValue>
glm::vec3 Shader<TValue>::SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2)
{
	glm::normalize(v1);
	v2 = glm::normalize(v2 - glm::dot(v1, v2) * v1);
	return glm::normalize(glm::cross(v1, v2));
}

template<typename TValue>
void Shader<TValue>::FillData(void* data)
{
	value = static_cast<TValue*>(data);
}

template<typename TValue>
Shader<TValue>::Shader():ShaderBase()
{
	value = nullptr;
}
template<typename TValue>
void Shader<TValue>::DefaultVertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	vertexOutContext.data[RegisterIndex::POSITION] = matrixContext->wvpMatrix * vertexInContext.data[RegisterIndex::POSITION];

	vertexOutContext.data[RegisterIndex::COLOR] = vertexInContext.data[RegisterIndex::COLOR];
}
template<typename TValue>
bool Shader<TValue>::DefaultPixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	pixelOutContext.color = Color(pixelInContext.data[RegisterIndex::COLOR]);
	pixelOutContext.depth = pixelInContext.depth;
	return false;
}
template<typename TValue>
void Shader<TValue>::DefaultGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)
{
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveInContext);
}
