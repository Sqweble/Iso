#pragma once

#include "cocos2d.h"

#include "base/CCVector.h"
#include "math/Mat4.h"

#include "Pattern.h"
#include "PatternVertex.h"

#include "GrowingArray.h"

#include "Classes/Polygon.h"


//class GeoPolygon;


USING_NS_CC;


class InputScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	
	void DrawPolygon(const GeoPolygon& aPoly, const Color4F& aLineColor, const Color4F& aVertexColor, const Color4F& aInOutCol , const Color4F& aOutInCol);
	void draw(Renderer* renderer, const Mat4& transform, uint32_t flags);


	void OnMouseDown(cocos2d::Event* aEvent);
	virtual bool onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);
	virtual void onTouchEnded(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);
	virtual void onTouchMoved(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);
	virtual void onTouchCancelled(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);
	CREATE_FUNC(InputScene);

private:
	void GenerateTreePattern();

	void UpdateRayInfo();

private:
	cocos2d::Label* myLabelTouchInfo;

	cocos2d::Scene* myPhysicsLayer;

	DrawNode* myDebugDrawNode;
	
	Vec2 myFirstRayPoint;
	Vec2 mySecondRayPoint;

	GrowingArray<PhysicsRayCastInfo> myRayCastInfo;

	Vec2 myRayInPoint;
	Vec2 myRayOutPoint;

	GrowingArray<Vec2> myPointCloud;

	PatternVertex* myRootPatterVertex;
	Pattern myPattern;


	GeoPolygon* myPolygon;

	GeoPolygon* myMousePolygon;

	GeoPolygon* myAandB;
	GeoPolygon* myAnotB;

	GrowingArray<GeoPolygon*> myPolygons;
	GrowingArray<Color4F> myPolyColors;
	GrowingArray<GrowingArray<cocos2d::Label*>> myPolyVertIndicesLabels;

	GrowingArray<GrowingArray<GeoVertex>> myNewPolygons;

	GrowingArray<Vec2> myRandomPointsIn;
	GrowingArray<Vec2> myRandomPointsOut;


};
