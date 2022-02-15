#pragma once
#include "include/context/VertexInContext.h"
#include "include/context/VertexOutContext.h"
#include "include/context/PixelInContext.h"
#include "include/context/PixelOutContext.h"
#include "include/material/Material.h"
#include "include/context/MatrixContext.h"
#include "include/context/LightContext.h"
class Shader
{
public:
	void VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext);
	void PixelShading(PixelInContext& vertexInContext, PixelOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext);
	glm::vec3 SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2);
};

