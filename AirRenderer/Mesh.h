#pragma once
#include "HalfEdgeFace.h"
#include "HalfEdgeVertex.h"
#include "HalfEdge.h"

class Mesh
{
public:
	HalfEdge* halfEdges;
	HalfEdgeFace* faces;
	HalfEdgeVertex* vertexs;
	Mesh();
	Mesh(HalfEdge* halfEdges, HalfEdgeFace* faces, HalfEdgeVertex* vertexs);
};

