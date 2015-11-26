

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "cocos2d.h"
USING_NS_CC;

#include "GameRoot.h"

#include "Polygon.h"

class GameObject : public cocos2d::Layer
{
public:
	bool init() {
		myDebugDrawNode = DrawNode::create();
		addChild(myDebugDrawNode); 
		return cocos2d::Layer::init(); };
	static GameObject* create(){ return nullptr; };

	void DrawPolygon(const GeoPolygon& aPoly, const Color4F& aColor);

protected:
	DrawNode* myDebugDrawNode;
private:

};


#endif