#pragma once
#include <include/context/VertexOutContext.h>
#include <vector>
enum class PrimitiveType
{
	POINT = 1,
	LINE = 2,
	TRIANGLE = 3
};
class PrimitiveContext
{
public:
	PrimitiveType primitiveType;
	int vertexIndexes[3];
};
class PrimitiveOutContextBuilder
{
public:
	std::vector<VertexOutContext>* vertexOutContexts;
	std::vector<PrimitiveContext>* primitiveOutContexts;
	PrimitiveOutContextBuilder(std::vector<VertexOutContext>& vertexOutContexts, std::vector<PrimitiveContext>& primitiveOutContexts)
	{
		this->vertexOutContexts = &vertexOutContexts;
		this->primitiveOutContexts = &primitiveOutContexts;
	}
	VertexOutContext GetVectorOutContext(int vertexIndex)
	{
		return vertexOutContexts->operator[](vertexIndex);
	}
	int AddVertexOutContext(VertexOutContext& vertexOutContext)
	{
		int vertexIndex = vertexOutContexts->size();
		vertexOutContexts->push_back(vertexOutContext);
		return vertexIndex;
	}
	void SubmitPrimitiveOutContext(PrimitiveContext& PrimitiveOutContext)
	{
		primitiveOutContexts->push_back(PrimitiveOutContext);
	}
};