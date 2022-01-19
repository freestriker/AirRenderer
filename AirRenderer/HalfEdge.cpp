#include "HalfEdge.h"

HalfEdge::HalfEdge()
{

}
HalfEdge::HalfEdge(HalfEdgeVertex* startVertex, HalfEdge* oppositeHalfEdge, HalfEdgeFace* face, HalfEdge* previousHalfEdge, HalfEdge* nextHalfEdge)
{
	this->startVertex = startVertex;
	this->oppositeHalfEdge = oppositeHalfEdge;
	this->face = face;
	this->previousHalfEdge = previousHalfEdge;
	this->nextHalfEdge = nextHalfEdge;
}
