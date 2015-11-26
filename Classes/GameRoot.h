#ifndef _GAME_ROOT_H_
#define _GAME_ROOT_H_

#include "cocos2d.h"

#include "GameScene.h"

class GameRoot : public cocos2d::Ref
{
	friend GameScene;
private:
	GameRoot();
	~GameRoot();
public:
	static GameRoot* GetInstance();
	
	static GameScene* GetGameScene();
	
private:

	GameScene* myGameScene;



};


#endif