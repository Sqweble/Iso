

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "cocos2d.h"

USING_NS_CC;

class GameObject : public cocos2d::Layer
{
public:
	bool init(){ return false; };
	static GameObject* create(){ return nullptr; };
private:

};


#endif