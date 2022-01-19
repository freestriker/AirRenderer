#include "HalfEdgeVertex.h"

HalfEdgeVertex::HalfEdgeVertex() :HalfEdgeVertex(glm::vec3(0, 0, 0), nullptr)
{

}

HalfEdgeVertex::HalfEdgeVertex(glm::vec3 position):HalfEdgeVertex(position, nullptr)
{

}

HalfEdgeVertex::HalfEdgeVertex(glm::vec3 position, HalfEdge* halfEdge)
{
	this->halfEdge = halfEdge;
	this->position = position;
}
