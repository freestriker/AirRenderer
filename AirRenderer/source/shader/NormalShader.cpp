#include <include/shader/NormalShader.h>

void NormalShader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	vertexOutContext.data[RegisterIndex::POSITION] = matrixContext->wvpMatrix * vertexInContext.data[RegisterIndex::POSITION];

	glm::vec3 normal = glm::normalize(glm::mat3(matrixContext->wv_tiMatrix) * glm::vec3(vertexInContext.data[RegisterIndex::NORMAL]));
	vertexOutContext.data[RegisterIndex::NORMAL] = glm::vec4(normal.x, normal.y, normal.z, 0);

	glm::vec3 tangent = glm::normalize(glm::mat3(matrixContext->wvMatrix) * glm::vec3(vertexInContext.data[RegisterIndex::TANGENT]));
	vertexOutContext.data[RegisterIndex::TANGENT] = glm::vec4(tangent.x, tangent.y, tangent.z, 0);

	vertexOutContext.data[RegisterIndex::COLOR] = vertexInContext.data[RegisterIndex::COLOR];

	vertexOutContext.data[RegisterIndex::TEXCOORD1] = vertexInContext.data[RegisterIndex::TEXCOORD1];
	//vPosition
	vertexOutContext.data[RegisterIndex::TEXCOORD2] = matrixContext->wvMatrix * vertexInContext.data[RegisterIndex::POSITION];
}

void NormalShader::PixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	glm::vec2 uv = glm::vec2(pixelInContext.data[RegisterIndex::TEXCOORD1]);
	Color c = Color(pixelInContext.data[RegisterIndex::COLOR]) * value->texture.Sample(uv);
	Color al = Color::black;
	Color dl = Color::black;
	Color sl = Color::black;

	glm::vec3 vPosition = pixelInContext.data[RegisterIndex::TEXCOORD2];
	glm::vec3 vNormal = pixelInContext.data[RegisterIndex::NORMAL];
	glm::vec3 vTangent = pixelInContext.data[RegisterIndex::TANGENT];
	glm::vec3 vBitangent = SchmidtOrthogonalization(vNormal, vTangent);
	glm::vec3 tNormal = value->normalMap.SampleNormal(uv);
	glm::mat3 tbnMatrix = glm::mat3(vTangent, vBitangent, vNormal);
	vNormal = glm::normalize((tbnMatrix * tNormal) + vNormal);
	for each (Light * light in lightContext->lights)
	{

		glm::vec3 lightDir = light->IncidentLight(vPosition, matrixContext->viewMatrix);
		if (glm::dot(-lightDir, vNormal) <= 0) continue;
		Color lc = light->ColorIntensity(lightDir);

		al = al + light->AmbintColorIntensity();

		float il_nCos = std::clamp(glm::dot(-lightDir, vNormal) / glm::distance(lightDir, glm::vec3(0, 0, 0)), float(0), float(1));
		dl = dl + lc * il_nCos;

		glm::vec3 ol = lightDir - 2 * glm::dot(lightDir, vNormal) * vNormal;
		glm::vec3 v = -vPosition;
		float ol_vCos = std::clamp(glm::dot(ol, v) / (glm::distance(ol, glm::vec3(0, 0, 0)) * glm::distance(v, glm::vec3(0, 0, 0))), float(0), float(1)) * 0.9;
		sl = sl + lc * std::powf(ol_vCos, value->convergenceExponent);
	}


	pixelOutContext.color = c * (al * value->ambientReflectance + dl * value->diffuseReflectance + sl * value->specularReflectance);
}

NormalShader::NormalShader():Shader<NormalData>()
{
}
