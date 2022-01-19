#pragma once
#include <glm/ext/vector_float3.hpp>
#include "HalfEdge.h"
class HalfEdgeVertex
{
public:
	glm::vec3 position;
	HalfEdge* halfEdge;
	HalfEdgeVertex();
	HalfEdgeVertex(glm::vec3 position);
	HalfEdgeVertex(glm::vec3 position, HalfEdge* halfEdge);
};

