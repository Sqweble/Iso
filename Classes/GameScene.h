#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	void menuCloseCallback(cocos2d::Ref* pSender);

	
	CREATE_FUNC(GameScene);

private:



};


#endif