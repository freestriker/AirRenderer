#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <include/context/PrimitiveContext.h>
#include <include/context/VertexOutContext.h>
class Clip
{
public:
	glm::vec4* clipPlanes;
	int clipPlaneCount;
	PrimitiveOutContextBuilder primitiveBuilder;
	std::vector<int> vertexIndexes;
	std::vector<VertexOutContext>* clipInVertexContext;
	std::vector< uint8_t> clipInVertexOutCodes;
	std::vector<PrimitiveContext>* clipInPrimitiveContext;
	std::vector<VertexOutContext>* clipOutVertexContext;
	std::vector<PrimitiveContext>* clipOutPrimitiveContext;

	Clip(glm::vec4* clipPlanes, int clipPlaneCount, std::vector<VertexOutContext>& clipInVertexContext, std::vector<PrimitiveContext>& clipInPrimitiveContext, std::vector<VertexOutContext>& clipOutVertexContext, std::vector<PrimitiveContext>& clipOutPrimitiveContext)
	{
		primitiveBuilder = PrimitiveOutContextBuilder(clipOutVertexContext, clipOutPrimitiveContext);
		this->clipPlanes = clipPlanes;
		this->clipPlaneCount = clipPlaneCount;
		vertexIndexes = std::vector<int>(6);
		this->clipInVertexContext = &clipInVertexContext;
		this->clipInPrimitiveContext = &clipInPrimitiveContext;
		this->clipOutVertexContext = &clipOutVertexContext;
		this->clipOutPrimitiveContext = &clipOutPrimitiveContext;
		clipInVertexOutCodes = std::vector< uint8_t>(clipInVertexContext.size());
		for (int i = 0, size = clipInVertexOutCodes.size(); i < size; i++)
		{
			clipInVertexOutCodes[i] = GetOutCode(clipInVertexContext[i].data[0], 0);
		}
	}
	uint8_t GetOutCode(const glm::vec4& v, const int startPlaneIndex)
	{
		uint8_t code = 0;
		for (int i = startPlaneIndex; i < clipPlaneCount; i++)
		{
			if (glm::dot(clipPlanes[i], v) < 0)
			{
				code |= (1 << i);
			}
		}
		return code;
	}
	uint8_t GetDirectOutCode(const glm::vec4& v, const int startPlaneIndex)
	{
		uint8_t code = 0;
		if (glm::dot(clipPlanes[startPlaneIndex], v) < 0)
		{
			code |= (1 << startPlaneIndex);
		}
		return code;
	}

	void ClipTriangle(int* vertexIndexes)
	{
		std::vector< VertexOutContext> clipVertexContexts = std::vector< VertexOutContext>(3);
		clipVertexContexts[0] = clipInVertexContext->operator[](vertexIndexes[0]);
		clipVertexContexts[1] = clipInVertexContext->operator[](vertexIndexes[1]);
		clipVertexContexts[2] = clipInVertexContext->operator[](vertexIndexes[2]);
		for (int i = 0; i < clipPlaneCount; i++)
		{
			std::vector< VertexOutContext> resultContext = std::vector< VertexOutContext>();
			for (int j = 0; j < clipVertexContexts.size(); j++)
			{
				VertexOutContext fromContext = clipVertexContexts[j];
				uint8_t fromCode = GetDirectOutCode(fromContext.data[0], i);
				VertexOutContext toContext = clipVertexContexts[(j + 1) % clipVertexContexts.size()];
				uint8_t toCode = GetDirectOutCode(toContext.data[0], i);
				if ((fromCode | toCode) == 0)//都在内
				{
					resultContext.push_back(toContext);
				}
				else if ((fromCode & toCode) != 0)//都在外同一侧
				{
				}
				else if (fromCode == 0)//内外
				{
					glm::vec4 fv = fromContext.data[0];
					glm::vec4 v = toContext.data[0];
					float fromR = glm::dot(clipPlanes[i], fv);
					float toR = glm::dot(clipPlanes[i], v);
					float a = -fromR / (toR - fromR);
					VertexOutContext endToContext = VertexOutContext::Lerp(fromContext, toContext, a);
					resultContext.push_back(endToContext);

				}
				else//外内
				{
					glm::vec4 fv = toContext.data[0];
					glm::vec4 v = fromContext.data[0];
					float toR = glm::dot(clipPlanes[i], fv);
					float fromR = glm::dot(clipPlanes[i], v);
					float a = -fromR / (toR - fromR);
					VertexOutContext endFromContext = VertexOutContext::Lerp(fromContext, toContext, a);
					resultContext.push_back(endFromContext);
					resultContext.push_back(toContext);
				}


			}
			clipVertexContexts = resultContext;
		}
		if (clipVertexContexts.size() > 2)
		{
			
			int p0 = primitiveBuilder.AddVertexOutContext(clipVertexContexts[0]);
			int pre = primitiveBuilder.AddVertexOutContext(clipVertexContexts[1]);

			PrimitiveContext pc = PrimitiveContext();
			pc.primitiveType = PrimitiveType::TRIANGLE;
			pc.vertexIndexes[0] = p0;

			for (int i = 2, size = clipVertexContexts.size(); i < size; i++)
			{
				int cur = primitiveBuilder.AddVertexOutContext(clipVertexContexts[i]);
				pc.vertexIndexes[1] = pre;
				pc.vertexIndexes[2] = cur;
				pre = cur;
				primitiveBuilder.SubmitPrimitiveOutContext(pc);
			}
		}

	}
	void ClipLine(int fromVertexIndex, int toVertexIndex)
	{
		VertexOutContext fromContext = clipInVertexContext->operator[](fromVertexIndex);
		uint8_t fromCode = clipInVertexOutCodes[fromVertexIndex];
		VertexOutContext toContext = clipInVertexContext->operator[](toVertexIndex);
		uint8_t toCode = clipInVertexOutCodes[toVertexIndex];
		if ((fromCode | toCode) == 0)//都在内
		{
			vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(fromContext));
			vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(toContext));
			return;
		}
		else if ((fromCode & toCode) != 0)//都在外同一侧
		{
			return;
		}
		else if (fromCode != 0 && toCode != 0)//都在外异侧
		{
			int i = 0;
			glm::vec4 fv = toContext.data[0];
			glm::vec4 v = fromContext.data[0];
			VertexOutContext endFromContext = fromContext;
			while (fromCode != 0 && i < clipPlaneCount)
			{
				if ((fromCode & (1 << i)) != 0)
				{
					float toR = glm::dot(clipPlanes[i], fv);
					float fromR = glm::dot(clipPlanes[i], v);
					float a = -fromR / (toR - fromR);
					if (0 < a && a < 1)
					{
						endFromContext = VertexOutContext::Lerp(endFromContext, toContext, a);
						v = endFromContext.data[0];
					}
				}
				i++;
				fromCode = GetOutCode(v, i);
			}

			i = 0;
			fv = endFromContext.data[0];
			v = toContext.data[0];
			VertexOutContext endToContext = toContext;
			while (toCode != 0 && i < clipPlaneCount)
			{
				if ((toCode & (1 << i)) != 0)
				{
					float fromR = glm::dot(clipPlanes[i], fv);
					float toR = glm::dot(clipPlanes[i], v);
					float a = -fromR / (toR - fromR);
					if (0 < a && a < 1)
					{
						endToContext = VertexOutContext::Lerp(endFromContext, endToContext, a);
						v = endToContext.data[0];
					}
				}
				i++;
				toCode = GetOutCode(v, i);
			}

			fromCode = GetOutCode(endFromContext.data[0], 0);
			toCode = GetOutCode(endToContext.data[0], 0);
			if ((fromCode | toCode) == 0)//都在内
			{
				vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(endFromContext));
				vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(endToContext));
				return;
			}
			else if ((fromCode & toCode) != 0)//都在外同一侧
			{
				return;
			}
			else//内外
			{
				throw "";
			}

		}
		else//内外
		{
			if (fromCode == 0)//内外
			{
				int i = 0;
				glm::vec4 fv = fromContext.data[0];
				glm::vec4 v = toContext.data[0];
				VertexOutContext endToContext = toContext;
				while (toCode != 0 )
				{
					if ((toCode & (1 << i)) == 1)
					{
						float fromR = glm::dot(clipPlanes[i], fv);
						float toR = glm::dot(clipPlanes[i], v);
						float a = -fromR / (toR - fromR);
						endToContext = VertexOutContext::Lerp(fromContext, endToContext, a);
						v = endToContext.data[0];
					}
					i++;
					toCode = GetOutCode(v, i);
				}
				vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(fromContext));
				vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(endToContext));
			}
			else//外内
			{
				int i = 0;
				glm::vec4 fv = toContext.data[0];
				glm::vec4 v = fromContext.data[0];
				VertexOutContext endFromContext = fromContext;
				while (fromCode != 0 )
				{
					if ((fromCode & (1 << i)) == 1)
					{
						float toR = glm::dot(clipPlanes[i], fv);
						float fromR = glm::dot(clipPlanes[i], v);
						float a = -fromR / (toR - fromR);
						endFromContext = VertexOutContext::Lerp(endFromContext, toContext, a);
						v = endFromContext.data[0];
					}
					i++;
					fromCode = GetOutCode(v, i);
				}
				vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(endFromContext));
				vertexIndexes.push_back(primitiveBuilder.AddVertexOutContext(toContext));
			}
		}
	}

	void ClipToNewPrimitive()
	{
		for each (PrimitiveContext primitiveContext in *clipInPrimitiveContext)
		{
			ClipPrimitive(primitiveContext);
		}
	}
	void ClipPrimitive(PrimitiveContext& primitiveContext)
	{
		vertexIndexes.clear();
		switch (primitiveContext.primitiveType)
		{
			case PrimitiveType::POINT:
			{
				glm::vec4 v = primitiveBuilder.vertexOutContexts->operator[](primitiveContext.vertexIndexes[0]).data[0];
				if (GetOutCode(v, 0) == 0)
				{
					primitiveBuilder.SubmitPrimitiveOutContext(primitiveContext);
				}
				break;
			}
			case PrimitiveType::LINE:
			{
				ClipLine(primitiveContext.vertexIndexes[0], primitiveContext.vertexIndexes[1]);
				if (vertexIndexes.size() == 2)
				{
					PrimitiveContext pc = PrimitiveContext();
					pc.primitiveType = PrimitiveType::LINE;
					pc.vertexIndexes[0] = vertexIndexes[0];
					pc.vertexIndexes[1] = vertexIndexes[1];
					primitiveBuilder.SubmitPrimitiveOutContext(pc);
				}
				break;
			}
			case PrimitiveType::TRIANGLE:
			{
				ClipTriangle(primitiveContext.vertexIndexes);
				break;
			}
		}
	}
};