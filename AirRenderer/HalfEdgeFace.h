#pragma once
#include "HalfEdge.h"
class HalfEdgeFace
{
public:
	HalfEdge* halfEdge;
	HalfEdgeFace();
	HalfEdgeFace(HalfEdge* halfEdge);
};

