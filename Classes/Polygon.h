#pragma once




#include "cocos2d.h"
USING_NS_CC;

#include "GrowingArray.h"



struct GeoVertex : public Vec2
{	
	GeoVertex()
		:Vec2()	
	{
	}
	GeoVertex(const Vec2& aVec2)
		:Vec2(aVec2)
	{}
	GeoVertex(const GeoVertex& aGeoVertex)
		:Vec2(aGeoVertex)
	{}

	inline const GeoVertex operator-(const Vec2& aVec2)	const
	{
		GeoVertex outVertex = *this;
		outVertex.x -= aVec2.x;
		outVertex.y -= aVec2.y;

		return outVertex;
	}

	//Color4F color;

};

struct GeoEdge
{
	GeoEdge()
	{}

	GeoEdge(const Vec2& aStart, const Vec2& aEnd)
		:myStart(aStart)
		,myEnd(aEnd)
	{
	}

	GeoEdge(const GeoVertex& aStart, const GeoVertex& aEnd)
		:myStart(aStart)
		,myEnd(aEnd)
	{
	}
	GeoVertex myStart;
	GeoVertex myEnd;
};

class GeoPolygon
{
public:
	void Init(const GrowingArray<GeoVertex>& someVertices);
	void Clear();

	void SetPositionToCenterOfVerices();
	void MakeVerticesLocalToPosition(const Vec2& aPosition);
	
	void GetEdges(GrowingArray<GeoEdge>& someOutEdges) const;
	void GetEdgesLocal(GrowingArray<GeoEdge>& someOutEdges) const;

	void SetPosition(const Vec2& aPosition);
	Vec2 GetPosition();
	
	GrowingArray<GeoVertex>& GetVertices();
	void GetVertices(GrowingArray<GeoVertex>& someOutVertices) const;
	void GetVerticesLocal(GrowingArray<GeoVertex>& someOutVertices) const;

	const int GetVerticesNum() const;

private:
	

	
	

private:
	//Should always be in order
	GrowingArray<GeoVertex> myVertices;
	Vec2 myPosition;

};

