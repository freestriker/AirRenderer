#include <include/shader/NormalShader.h>

void NormalShader::VertexShading(VertexInContext& vertexInContext, VertexOutContext& vertexOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	vertexOutContext.data[RegisterIndex::POSITION] = matrixContext->wvpMatrix * vertexInContext.data[RegisterIndex::POSITION];

	glm::vec3 normal = glm::normalize(glm::mat3(matrixContext->wv_itMatrix) * glm::vec3(vertexInContext.data[RegisterIndex::NORMAL]));
	vertexOutContext.data[RegisterIndex::NORMAL] = glm::vec4(normal.x, normal.y, normal.z, 0);

	glm::vec3 tangent = glm::normalize(glm::mat3(matrixContext->wvMatrix) * glm::vec3(vertexInContext.data[RegisterIndex::TANGENT]));
	vertexOutContext.data[RegisterIndex::TANGENT] = glm::vec4(tangent.x, tangent.y, tangent.z, 0);

	vertexOutContext.data[RegisterIndex::COLOR] = vertexInContext.data[RegisterIndex::COLOR];

	vertexOutContext.data[RegisterIndex::TEXCOORD1] = vertexInContext.data[RegisterIndex::TEXCOORD1];
	//vPosition
	vertexOutContext.data[RegisterIndex::TEXCOORD2] = matrixContext->wvMatrix * vertexInContext.data[RegisterIndex::POSITION];
}

bool NormalShader::PixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	glm::vec2 uv = glm::vec2(pixelInContext.data[RegisterIndex::TEXCOORD1]);
	Color c = Color(pixelInContext.data[RegisterIndex::COLOR]) * value->texture.Sample(uv);
	Color al = Color::black;
	Color dl = Color::black;
	Color sl = Color::black;

	glm::vec3 vPosition = pixelInContext.data[RegisterIndex::TEXCOORD2];
	glm::vec3 vNormal = pixelInContext.data[RegisterIndex::NORMAL];
	vNormal = glm::normalize(vNormal);
	glm::vec3 vTangent = pixelInContext.data[RegisterIndex::TANGENT];
	vTangent = glm::normalize(vTangent);
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
	pixelOutContext.depth = pixelInContext.depth;
	return false;
}
void NormalShader::GeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)
{
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveInContext);
}

bool NormalShader::WireframePixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	pixelOutContext.color = Color::green;
	pixelOutContext.depth = pixelInContext.depth;
	return false;
}

void NormalShader::WireframeGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)
{
	PrimitiveContext primitiveOutContext = primitiveInContext;

	primitiveOutContext.primitiveType = PrimitiveType::LINE;
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveOutContext);

	int* indexes = primitiveInContext.vertexIndexes;

	primitiveOutContext.vertexIndexes[0] = indexes[1];
	primitiveOutContext.vertexIndexes[1] = indexes[2];
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveOutContext);

	primitiveOutContext.vertexIndexes[0] = indexes[2];
	primitiveOutContext.vertexIndexes[1] = indexes[0];
	primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveOutContext);
}
bool NormalShader::NormalPixelShading(PixelInContext& pixelInContext, PixelOutContext& pixelOutContext, MatrixContext* matrixContext, LightContext* lightContext)
{
	pixelOutContext.color = Color::red;
	pixelOutContext.depth = pixelInContext.depth;
	return false;
}

void NormalShader::NormalGeometryShading(PrimitiveContext& primitiveInContext, PrimitiveOutContextBuilder& primitiveOutContextBuilder, MatrixContext* matrixContext, LightContext* lightContext)
{
	PrimitiveContext primitiveOutContext = primitiveInContext;

	primitiveOutContext.primitiveType = PrimitiveType::LINE;

	for (int i = 0; i < 3; i++)
	{
		VertexOutContext newVertexOutContext = primitiveOutContextBuilder.vertexOutContexts->operator[](primitiveInContext.vertexIndexes[i]);
		glm::vec3 vNormal = newVertexOutContext.data[RegisterIndex::NORMAL];
		glm::vec3 vPosition = newVertexOutContext.data[RegisterIndex::TEXCOORD2];
		vPosition += vNormal * float(1.0);
		glm::vec4 pPosition = matrixContext->projectionMatrix * glm::vec4(vPosition, 1.0);

		newVertexOutContext.data[RegisterIndex::POSITION] = pPosition;
		primitiveOutContext.vertexIndexes[0] = primitiveInContext.vertexIndexes[i];
		primitiveOutContext.vertexIndexes[1] = primitiveOutContextBuilder.AddVertexOutContext(newVertexOutContext);
		primitiveOutContextBuilder.SubmitPrimitiveOutContext(primitiveOutContext);
	}
}

NormalShader::NormalShader():Shader<NormalData>()
{
	this->activeTable[0] = true;
	this->shaderPasses[0].vertexShading = std::bind(&NormalShader::VertexShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	this->shaderPasses[0].pixelShading = std::bind(&NormalShader::NormalPixelShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	this->shaderPasses[0].geometryShading = std::bind(&NormalShader::NormalGeometryShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	
	this->activeTable[1] = true;
	this->shaderPasses[1].vertexShading = std::bind(&NormalShader::VertexShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	this->shaderPasses[1].pixelShading = std::bind(&NormalShader::WireframePixelShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	this->shaderPasses[1].geometryShading = std::bind(&NormalShader::WireframeGeometryShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	
	this->activeTable[2] = true;
	this->shaderPasses[2].vertexShading = std::bind(&NormalShader::VertexShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	this->shaderPasses[2].pixelShading = std::bind(&NormalShader::PixelShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	this->shaderPasses[2].geometryShading = std::bind(&NormalShader::GeometryShading, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}
