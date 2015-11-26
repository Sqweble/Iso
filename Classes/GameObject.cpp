
#include "GameObject.h"
void GameObject::DrawPolygon(const GeoPolygon& aPoly, const Color4F& aColor)
{
	GrowingArray<GeoEdge> polyEdges(4);
	aPoly.GetEdgesWorld(polyEdges);


	GrowingArray<GeoVertex> verts(4);
	aPoly.GetVerticesWorld(verts);

	//	myDebugDrawNode->drawSolidPoly(verts.GetArray(), verts.Count(), aLineColor);
	for (int i = 0; i < polyEdges.Count(); ++i)
	{

		myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(polyEdges[i].myStart), myDebugDrawNode->convertToNodeSpace(polyEdges[i].myEnd), aColor);
		Vec2 line = (polyEdges[i].myEnd - polyEdges[i].myStart);
		Vec2 center = polyEdges[i].myStart + line / 2;
		Vec2 centrtOffSet = line.getNormalized();
		centrtOffSet.rotate(Vec2::ZERO, -(float)M_PI_2);
		centrtOffSet *= 15.f;
		myDebugDrawNode->drawPoint(myDebugDrawNode->convertToNodeSpace(center), 2, aColor);
		myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(center), myDebugDrawNode->convertToNodeSpace(center + centrtOffSet), aColor);


	}

}