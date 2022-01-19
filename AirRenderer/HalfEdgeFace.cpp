#include "HalfEdgeFace.h"

HalfEdgeFace::HalfEdgeFace():HalfEdgeFace(nullptr)
{

}

HalfEdgeFace::HalfEdgeFace(HalfEdge* halfEdge)
{
	this->halfEdge = halfEdge;
}
