#pragma once
#include <include/shader/ShaderBase.h>

template<typename TValue>
class Shader:public ShaderBase
{
public:
	TValue* value;
	virtual void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext)override = 0;
	virtual void PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext)override = 0;
	virtual void GeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)override;
	glm::vec3 SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2);
	void FillData(void* data)override;
	Shader();
};

template<typename TValue>
void Shader<TValue>::GeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)
{
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveInContext);
}

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
