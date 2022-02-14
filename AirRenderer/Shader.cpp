#include "Shader.h"
#include "Texture.h"
#include <algorithm>

void Shader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext)
{
	vertexOutContext.pPosition = matrixContext->wvpMatrix * vertexInContext.mPosition;
	vertexOutContext.vPosition = matrixContext->wvMatrix * vertexInContext.mPosition;
	vertexOutContext.wPosition = matrixContext->worldMatrix * vertexInContext.mPosition;

	vertexOutContext.vNormal = glm::normalize(glm::mat3(matrixContext->wv_tiMatrix) * vertexInContext.mNormal);
	vertexOutContext.wNormal = glm::normalize(glm::mat3(matrixContext->w_tiMatrix) * vertexInContext.mNormal);

	vertexOutContext.vTangent = glm::normalize(glm::mat3(matrixContext->wvMatrix) * vertexInContext.mTangent);
	vertexOutContext.wTangent = glm::normalize(glm::mat3(matrixContext->worldMatrix) * vertexInContext.mTangent);

	vertexOutContext.texcoord1 = vertexInContext.texcoord1;
	vertexOutContext.color = vertexInContext.color;
}

void Shader::PixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, Material& material, MatrixContext* matrixContext, LightContext* lightContext)
{
	Color c = pixelInContext.color * static_cast<Texture*>(material.texture)->Sample(pixelInContext.texcoord1);
	Color al = Color::black;
	Color dl = Color::black;
	Color sl = Color::black;

	glm::vec3 vPosition = pixelInContext.vPosition;
	glm::vec3 vNormal = pixelInContext.vNormal;
	glm::vec3 vTangent = pixelInContext.vTangent;
	glm::vec3 vBitangent = SchmidtOrthogonalization(vNormal, vTangent);
	glm::vec3 tNormal = static_cast<Texture*>(material.normalMap)->SampleNormal(pixelInContext.texcoord1);
	glm::mat3 tbnMatrix = glm::mat3(vTangent, vBitangent, vNormal);
	vNormal = glm::normalize((tbnMatrix * tNormal) + vNormal);

	for each (Light* light in lightContext->lights)
	{

		glm::vec3 lightDir = light->IncidentLight(vPosition, matrixContext->viewMatrix);
		if (glm::dot(-lightDir, vNormal) <= 0) continue;
		Color lc = light->ColorIntensity(lightDir);

		al = al + light->AmbintColorIntensity();

		float il_nCos = std::clamp(glm::dot(-lightDir, vNormal) / glm::distance(lightDir, glm::vec3(0, 0, 0)), float(0), float(1));
		dl = dl + lc * il_nCos;

		glm::vec3 ol = lightDir - 2 * glm::dot(lightDir, vNormal) * vNormal;
		glm::vec3 v =  -vPosition;
		float ol_vCos = std::clamp(glm::dot(ol, v) / (glm::distance(ol, glm::vec3(0, 0, 0)) * glm::distance(v, glm::vec3(0, 0, 0))), float(0), float(1)) * 0.9;
		sl = sl + lc * std::powf(ol_vCos, material.convergenceExponent);
	}

	//glm::vec3 wPosition = pixelInContext.wPosition;
	//glm::vec3 wNormal = pixelInContext.wNormal;

	//glm::vec3 wTangent = pixelInContext.wTangent;
	//glm::vec3 wBitangent = SchmidtOrthogonalization(wNormal, wTangent);
	//glm::vec3 tNormal = static_cast<Texture*>(material.normalMap)->SampleNormal(pixelInContext.texcoord1);
	//glm::mat3 tbnMatrix = glm::mat3(wTangent, wBitangent, wNormal);
	//wNormal = glm::normalize(tbnMatrix * tNormal/* * wNormal*/);
	//for each (Light * light in lightContext->lights)
	//{

	//	glm::vec3 lightDir = light->IncidentLight(wPosition);
	//	if (glm::dot(-lightDir, wNormal) <= 0) continue;
	//	Color lc = light->ColorIntensity(lightDir);

	//	al = al + light->AmbintColorIntensity();

	//	float il_nCos = std::clamp(glm::dot(-lightDir, wNormal) / glm::distance(lightDir, glm::vec3(0, 0, 0)), float(0), float(1));
	//	dl = dl + lc * il_nCos;

	//	glm::vec3 ol = lightDir - 2 * glm::dot(lightDir, wNormal) * wNormal;
	//	glm::vec3 v = -wPosition;
	//	float ol_vCos = std::clamp(glm::dot(ol, v) / (glm::distance(ol, glm::vec3(0, 0, 0)) * glm::distance(v, glm::vec3(0, 0, 0))), float(0), float(1)) * 0.9;
	//	sl = sl + lc * std::powf(ol_vCos, material.convergenceExponent);
	//}

	pixelOutContext.color = c * (al * material.ambientReflectance + dl * material.diffuseReflectance + sl * material.specularReflectance);
}

glm::vec3 Shader::SchmidtOrthogonalization(glm::vec3& v1, glm::vec3& v2)
{
	glm::normalize(v1);
	v2 = glm::normalize(v2 - glm::dot(v1, v2) * v1);
	return glm::normalize(glm::cross(v1, v2));
}
