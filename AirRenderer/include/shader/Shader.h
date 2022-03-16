#pragma once
#include <include/utils/RegisterIndex.h>
#include <include/shader/ShaderOptions.h>
#include <functional>
#include <include/context/VertexInContext.h>
#include <include/context/PixelOutContext.h>
#include <include/context/PixelInContext.h>
#include <include/context/VertexOutContext.h>
#include <include/context/MatrixContext.h>
#include <include/context/LightContext.h>
#include <include/context/PrimitiveContext.h>
#include <include/shader/ShaderData.h>

class ShaderPass
{
public:
	std::string passName;
	ShaderOption shaderOption;
	std::function<void(VertexInContext&, VertexOutContext&, MatrixContext*, LightContext*)> vertexShading;
	std::function<bool(PixelInContext&, PixelOutContext&, MatrixContext*, LightContext*)> pixelShading;
	std::function<void(PrimitiveContext&, PrimitiveOutContextBuilder&, MatrixContext*, LightContext*)> geometryShading;

	ShaderPass();
	~ShaderPass();
};
class Shader:public Object
{
public:
	std::vector< ShaderPass> shaderPasses;
	ShaderData* value;
	Shader();
	virtual ~Shader();
	virtual Shader* Clone() = 0;
	static glm::vec3 SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2);
	void DefaultVertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	bool DefaultPixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext);
	void DefaultGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext);
	void FillData(ShaderData* value);
	template<typename T>
	T* Data();
};

template<typename T>
T* Shader::Data()
{
	return static_cast<T*>(value);
}
