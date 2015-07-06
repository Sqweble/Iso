#include "PatternEdge.h"

#include "PatternVertex.h"

#include "2d\CCDrawNode.h"


PatternEdge::PatternEdge()
{
}

PatternEdge::~PatternEdge()
{
}


PatternEdge* PatternEdge::Create()
{
	PatternEdge* edge = new (std::nothrow) PatternEdge();
	if (edge)
	{
		//edge->autorelease();
		return edge;
	}

	CC_SAFE_DELETE(edge);
	return nullptr;
}

bool PatternEdge::CanConnect()
{
	return (!myVertices.first || !myVertices.second);	
}
bool PatternEdge::IsConnectedWithVertex(PatternVertex* aVertex)
{
	return (myVertices.first == aVertex || myVertices.second == aVertex);
}

bool PatternEdge::ConnectVertices(PatternVertex* aFirstVert, PatternVertex* aSecondVert)
{
	if ( !myVertices.first && !myVertices.second)
	{
		aFirstVert->ConnectEdge(this);
		aSecondVert->ConnectEdge(this);
		
		return true;
	}
	return false;
}

void PatternEdge::ConnectVertex(PatternVertex* aVertex)
{
	if (!myVertices.first)
		myVertices.first = aVertex;
	else if (!myVertices.second)
		myVertices.second = aVertex;
}
void PatternEdge::DisconnectVertex(PatternVertex* aVertex)
{
	if (myVertices.first == aVertex)
		myVertices.first = nullptr;
	else if (myVertices.second == aVertex)
		myVertices.second = nullptr;
}

PatternVertex* PatternEdge::GetConnectedVertex(PatternVertex* aVertex)
{
	if (myVertices.first == aVertex)
		return myVertices.second;
	else if (myVertices.second == aVertex)
		return myVertices.first;

	return nullptr;
}

void PatternEdge::DrawHierarchly(DrawNode* aDrawNode, float& Depth)
{	
	myVertices.first->DrawHierarchly(aDrawNode, Depth);

	if (myVertices.first && myVertices.second)
	{
		aDrawNode->drawLine(myVertices.first->GetPosition(), myVertices.second->GetPosition(), Color4F::ORANGE);
	}	

	Depth++;
}