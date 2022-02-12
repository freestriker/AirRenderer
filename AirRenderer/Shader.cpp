#include "Shader.h"
#include "Texture.h"
#include <algorithm>

void Shader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext)
{
	vertexOutContext.position = matrixContext->wvpMatrix * vertexInContext.position;
	vertexOutContext.worldPosition = matrixContext->wvMatrix * vertexInContext.position;
	vertexOutContext.texcoord1 = vertexInContext.texcoord1;
	vertexOutContext.normal = glm::mat3(matrixContext->wv_tiMatrix) * vertexInContext.normal;
	vertexOutContext.color = vertexInContext.color;
}

void Shader::PixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext)
{
	Color c = pixelInContext.color * static_cast<Texture*>(material.texture)->Sample(pixelInContext.texcoord1);
	Color al = Color::black;
	Color dl = Color::black;
	Color sl = Color::black;
	glm::vec3 pos = pixelInContext.worldPosition;
	for each (Light* light in lightContext->lights)
	{

		glm::vec3 lightDir = light->IncidentLight(pos, matrixContext->viewMatrix);
		if (glm::dot(-lightDir, pixelInContext.normal) <= 0) continue;
		Color lc = light->ColorIntensity(lightDir);

		al = al + light->AmbintColorIntensity();

		float il_nCos = std::clamp(glm::dot(-lightDir, pixelInContext.normal) / glm::distance(lightDir, glm::vec3(0, 0, 0)), float(0), float(1));
		dl = dl + lc * il_nCos;

		glm::vec3 ol = lightDir - 2 * glm::dot(lightDir, pixelInContext.normal) * pixelInContext.normal;
		glm::vec3 v =  -pos;
		float ol_vCos = std::clamp(glm::dot(ol, v) / (glm::distance(ol, glm::vec3(0, 0, 0)) * glm::distance(v, glm::vec3(0, 0, 0))), float(0), float(1)) * 0.9;
		sl = sl + lc * std::powf(ol_vCos, material.convergenceExponent);

	}
	pixelOutContext.color = c * (al * material.ambientReflectance + dl * material.diffuseReflectance + sl * material.specularReflectance);
}
