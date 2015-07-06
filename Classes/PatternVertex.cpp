#include "PatternVertex.h"

#include "PatternEdge.h"


PatternVertex::PatternVertex()
{
}

PatternVertex::~PatternVertex()
{
}

PatternVertex* PatternVertex::Create()
{
	PatternVertex* vert = new (std::nothrow) PatternVertex();
	if (vert)
	{
		//vert->autorelease();
		return vert;
	}

	CC_SAFE_DELETE(vert);
	return nullptr;
}


void PatternVertex::SetPosition(const Vec2& aPosition)
{
	myPosition = aPosition;
}
const Vec2 PatternVertex::GetPosition() const
{
	return myPosition;
}


bool PatternVertex::ConnectEdge(PatternEdge* aEdge)
{
	if (aEdge->CanConnect() && !aEdge->IsConnectedWithVertex(this))
	{
		aEdge->ConnectVertex(this);
		myEdges.push_back(aEdge);
	}
	return false;
}


void PatternVertex::DrawHierarchly(DrawNode* aDrawNode, float& aDepth)
{
	//if (myEdges.size() > 0)
	//{
	//	for (PatternEdge* it : myEdges)
	//	{
	//		PatternVertex* vert = it->GetConnectedVertex(this);
	//		if (vert)
	//		{
	//			aDrawNode->drawLine(GetPosition(), vert->GetPosition(),Color4F::ORANGE);				
	//			vert->DrawHierarchly(aDrawNode, aDepth);
	//		}
	//	}
	//}
	//
	//
	//Vec2 pos = myPosition;
	//aDrawNode->drawDot(pos, 8.f, Color4F::BLUE);
}