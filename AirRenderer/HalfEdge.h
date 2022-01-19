#pragma once
#include "HalfEdgeFace.h"
#include "HalfEdgeVertex.h"

class HalfEdge
{
public:
	HalfEdgeVertex* startVertex;
	HalfEdge* oppositeHalfEdge;
	HalfEdgeFace* face;
	HalfEdge* previousHalfEdge;
	HalfEdge* nextHalfEdge;
	HalfEdge();
	HalfEdge(HalfEdgeVertex* startVertex, HalfEdge* oppositeHalfEdge, HalfEdgeFace* face, HalfEdge* previousHalfEdge, HalfEdge* nextHalfEdge);
};

