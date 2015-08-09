#include "GameScene.h"

#include "physics/CCPhysicsWorld.h"
#include "2d/CCScene.h"

#include "ScreenInfo.h"

#include "Player.h"
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

	Player* player = Player::create();

	addChild(player);


	auto ground = Node::create();

	Vec2 startPos = ScreenInfo::leftBottom() + Vec2(0, 100) + Vec2(-10000,0);

	Vec2 points[9];
	points[0] = startPos;
	points[1] = ScreenInfo::leftBottom() + Vec2(0, 100);
	points[2] = points[1] + Vec2(200, 0);
	points[3] = points[2] + Vec2(200, -50);
	points[4] = points[3] + Vec2(200, 0);
	points[5] = points[4] + Vec2(0, 10);
	points[6] = points[5] + Vec2(100, 0);
	points[7] = points[6] + Vec2(0, 32);
	points[8] = points[7] + Vec2(10000, 0);
	
	ground->setPhysicsBody(PhysicsBody::createEdgeChain(points, 9));
	//ground->setPhysicsBody(PhysicsBody::createEdgeSegment(ScreenInfo::leftBottom() + Vec2(0, 100), ScreenInfo::rightBottom() + Vec2(0, 100)));
	addChild(ground);


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
