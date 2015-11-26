#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_


#define PTM_RATIO 64.0

#include "cocos2d.h"
USING_NS_CC;

#include "GrowingArray.h"

class DestructibleObject;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	
	void menuCloseCallback(cocos2d::Ref* pSender);

	
	CREATE_FUNC(GameScene);


	GrowingArray<DestructibleObject*> myDestructibleObjects;
private:
	class DrawNode* myDebugDrawNode;


};


#endif