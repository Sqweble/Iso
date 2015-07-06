#include "GameScene.h"

#include "physics/CCPhysicsWorld.h"
#include "2d/CCScene.h"

USING_NS_CC;


Scene* GameScene::createScene()
{
	Scene* scene = Scene::createWithPhysics();
	Layer* gameSceneLayer = GameScene::create();

	scene->addChild(gameSceneLayer);
	
	//Debug
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	return scene;
}

bool GameScene::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	Label* label = Label::createWithSystemFont("Gamescene Works", "Arial", 96);

	label->setAnchorPoint(Vec2(0.0, 0.0));
	addChild(label);


	return true;
	//Create game layerrs
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
