#pragma once
#include "VertexInContext.h"
#include "VertexOutContext.h"
#include "PixelInContext.h"
#include "PixelOutContext.h"
#include "Material.h"
#include "MatrixContext.h"
#include "LightContext.h"
class Shader
{
public:
	void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext);
	void PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext);
	glm::vec3 SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2);
};

