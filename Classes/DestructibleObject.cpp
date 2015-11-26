#include "DestructibleObject.h"

#include "ScreenInfo.h"

#include "PolygonOperations.h"


bool DestructibleObject::init()
{
	if (!GameObject::init())
		return false;

	myPolygon = GeoPolygon::create();
	addChild(myPolygon);
	
	myPhysicsBody = PhysicsBody::create();	
	setPhysicsBody(myPhysicsBody);
	

	

	return true;

}
DestructibleObject* DestructibleObject::create()
{
	DestructibleObject* ret = new (std::nothrow) DestructibleObject();
	if (ret && ret->init())
	{
		ret->autorelease();
		GameRoot::GetGameScene()->addChild(ret);
		GameRoot::GetGameScene()->myDestructibleObjects.Add(ret);
		ret->scheduleUpdate();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}
void DestructibleObject::Init(const GrowingArray<GeoVertex>& aPolygon)
{	
	setPosition(GeoPolygon::GetCenterOfWorldVertices(aPolygon));
	myPolygon->SetWorldVertices(aPolygon);
	
	GrowingArray<GeoVertex> localPolygonPoints(myPolygon->GetVerticesNum());
	myPolygon->GetVerticesLocal(localPolygonPoints);

	cocos2d::Vec2* polygonPointsArray = localPolygonPoints.GetArray();

	PhysicsBody* physicBody = myPhysicsBody;
	physicBody->removeAllShapes();	

	PhysicsMaterial physMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT;
	physMaterial.friction = 50.f;
	physMaterial.density = 10.f;
		
	physicBody->addShape(PhysicsShapePolygon::create(polygonPointsArray, myPolygon->GetVerticesNum(), physMaterial));

	
	
}
void  DestructibleObject::update(float aDeltaTime)
{

}
void DestructibleObject::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	myDebugDrawNode->clear();

	//myDebugDrawNode->drawPoint(myDebugDrawNode->convertToNodeSpace(Vec2(-32, 0)), 8, Color4F::BLUE);
	//myDebugDrawNode->drawPoint(myDebugDrawNode->convertToNodeSpace(Vec2(32, 0)), 8, Color4F::RED);	
	//myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(myPhysicsBody->getPosition()), 16, 0, 8, false, Color4F::YELLOW);
	//myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(getPosition()), 24, 0, 8, false, Color4F::MAGENTA);	
	//myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(convertToWorldSpace(myPolygon->getPosition())), 32, 0, 8, false, Color4F::GREEN);

	
	DrawPolygon(*myPolygon, Color4F::GREEN);
}

void DestructibleObject::Cut(GeoPolygon* aCuttingPolygon)
{
	bool clipping = GeoPolygonOperations::WeilerAthertonClipping(*myPolygon, *aCuttingPolygon, myNewPolygons, EClippingMode::AnotB);
	if (clipping && myNewPolygons.Count() >= 2)
	{		
		Init(myNewPolygons[0]);
	
		for (int i = 1; i < myNewPolygons.Count(); i++)
		{
			DestructibleObject* newDestrObj = DestructibleObject::create();	
			newDestrObj->Init(myNewPolygons[i]);			
		}
	}
}
const Mat4&  DestructibleObject::getNodeToParentTransform() const
{
	Node::getNodeToParentTransform();

 	getPosition(&_transform.m[12], &_transform.m[13]);
	return _transform;
}
