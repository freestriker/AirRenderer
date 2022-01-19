#include "Mesh.h"

Mesh::Mesh() :Mesh(nullptr, nullptr, nullptr)
{

}

Mesh::Mesh(HalfEdge* halfEdges, HalfEdgeFace* faces, HalfEdgeVertex* vertexs)
{
	this->halfEdges = halfEdges;
	this->faces = faces;
	this->vertexs = vertexs;
}
