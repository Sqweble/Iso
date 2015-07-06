#include "Pattern.h"

#include "PatternEdge.h"
#include "PatternVertex.h"


Pattern::Pattern()
{
}

Pattern::~Pattern()
{
}
void Pattern::CreateNewVertex(PatternVertex*& aOutVertex)
{
	aOutVertex = CreateVertex();
}
void Pattern::CreateNewVertex(PatternVertex*& aOutVertex, PatternEdge*& aOutEdge, PatternVertex* aVertexToConnectTo)
{
	PatternVertex* vert = CreateVertex();
	PatternEdge* edge = CreateEdge();
	edge->ConnectVertices(aVertexToConnectTo, vert);

	aOutVertex = vert;
	aOutEdge = edge;
}

PatternEdge* Pattern::CreateEdge()
{
	PatternEdge* edge = PatternEdge::Create();
	myEdges.push_back(edge);
	return edge;
}

PatternVertex* Pattern::CreateVertex()
{
	PatternVertex* vert = PatternVertex::Create();
	myVertices.push_back(vert);
	return vert;
}