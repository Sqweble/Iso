#pragma once

#include "Polygon.h"

#include "StaticArray.h"


namespace EClippingMode
{
	enum Type
	{
		AandB,	
		AnotB,	
		BnotA,	
		AunionB,
		AxorB,
		Num,
	};
};
namespace EClippingLoop
{
	enum Type
	{
		Subject,
		Clip,
		Num,
	};
};
namespace EClippingLoopDirection
{
	enum Type
	{
		Backward,
		Forward,
		Num,
	};
};

namespace EClippingIntersectionType
{
	enum Type
	{
		InOut,
		OutIn,
		None,
	};
};

struct GeoVertexClipNode
{
	GeoVertexClipNode()
		:myVertex()
		, myIsIntersectionPoint(false)
		, myIsVisited(false)
		, myClipType(EClippingIntersectionType::None)
	{}
	GeoVertexClipNode(const Vec2& aVec2)
		: myVertex(aVec2)
		, myIsIntersectionPoint(false)
		, myIsVisited(false)
		, myClipType(EClippingIntersectionType::None)
	{}
	GeoVertexClipNode(const GeoVertex& aVertex)
		: myVertex(aVertex)
		, myIsIntersectionPoint(false)
		, myIsVisited(false)
		, myClipType(EClippingIntersectionType::None)
	{}


	GeoVertex myVertex;

	bool myIsIntersectionPoint;
	bool myIsVisited;
	EClippingIntersectionType::Type myClipType;
	StaticArray<StaticArray<GeoVertexClipNode*, 2>, EClippingLoop::Num> myClippingLoops;


};

struct GeoVertexClipNodeIntersectionSortData
{
	int myIndex;
	float myLengthFromLineStart;
	GeoVertexClipNode myNode;
};


class GeoPolygonOperations
{
public:
	static bool Inside(const Vec2& aPoint, const Vec2& aStart, const Vec2& aEnd);
	static bool Inside(const Vec2& aPoint, const GrowingArray<GeoEdge>& aEdgeSegment);
	static bool Inside(const Vec2& aPoint, const GeoPolygon& aPolygon);

	static void AandB(const GeoPolygon& aPolyA, const GeoPolygon& aPolyB, GeoPolygon& aOutPoly);

	static void AnotB(const GeoPolygon& aPolyA, const GeoPolygon& aPolyB, GeoPolygon& aOutPoly);

	
	static bool WeilerAthertonClipping(GeoPolygon& aSubjectPolygon, GeoPolygon& aClipPolygon, GrowingArray<GrowingArray<GeoVertex>>& outNewPolys, EClippingMode::Type aClippingMode);
	
	





};