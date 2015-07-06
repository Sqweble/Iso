#include "GraphicsScene.h"


USING_NS_CC;

Scene* GraphicsScene::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = GraphicsScene::create();

	scene->addChild(layer);

	return scene;
}


bool GraphicsScene::init()
{
	if (!Layer::init())
		return false;	

	Sprite* sprite = Sprite::create("CloseNormal.png");

	Sprite* sprite2 = Sprite::create("CloseNormal.png");
	Sprite* sprite3 = Sprite::create("CloseNormal.png");

	sprite2->setPosition(20, 20);

	sprite->setAnchorPoint(Vec2(0.5, 0.5));
		

	sprite->addChild(sprite2);
	sprite2->addChild(sprite3);

	sprite->setPosition(300, 200);
	addChild(sprite);

	return true;
}