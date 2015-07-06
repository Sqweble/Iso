#include "HelloWorldScene.h"

#include "physics/CCPhysicsWorld.h"
#include "2d/CCScene.h"

USING_NS_CC;




Scene* HelloWorld::createScene()
{
	Scene* scene = Scene::createWithPhysics();
	Layer* layer = HelloWorld::create();

	scene->addChild(layer);
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	

	return scene;

}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
   if (!Layer::init())
   {
	   return false;
   }

   Label* label = Label::createWithSystemFont("Hello world", "Arial", 96);

   label->setAnchorPoint(Vec2(0.0, 0.0));

   addChild(label);

   return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
