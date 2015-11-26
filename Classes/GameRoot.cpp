#include "GameRoot.h"


#include <string>


static GameRoot* sGameRoot = nullptr;

GameRoot::GameRoot()
{
}

GameRoot::~GameRoot()
{
}

GameRoot* GameRoot::GetInstance()
{
	if (sGameRoot == nullptr)
	{
		sGameRoot = new (std::nothrow) GameRoot();
		//CCASSERT(sGameRoot, "FATAL: Not enough memory");		
	}
	return sGameRoot;
}
GameScene* GameRoot::GetGameScene()
{
	return GetInstance()->myGameScene;
}