#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "GameObject.h"

#include <bitset>

#include "Polygon.h"
#include "PolygonOperations.h"

USING_NS_CC;


namespace EMovementDirection
{
	enum Type
	{
		Left,
		Right,
		None
	};
}

namespace EInputMapping
{
	enum Type
	{
		Move_Left,
		Move_Right,
		Move_Up,
		Move_Down,
		Jump,
		Num,
	};
}
class Player : public GameObject
{
public:
	Player();
	~Player();

	bool init();
	static Player* create();


	void update(float aDeltaTime);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	

	//Input
	void OnKeyPressed(EventKeyboard::KeyCode aKeyCode, Event* aEvent);
	void OnKeyReleased(EventKeyboard::KeyCode aKeyCode, Event* aEvent);

	//RayCast
	bool OnRayCastIntersect(PhysicsWorld& aWorld, const PhysicsRayCastInfo& someInfo, void* someData);
	

	void UpdateNormalMovement(const float aDeltaTime, Vec2& aInOutMovementVector);
	void UpdateDashingMovement(const float aDeltaTime, Vec2& aInOutMovementVector);

	void UpdateGravityMovement(const float aDeltaTime, Vec2& aInOutMovementVector);

	void AddMovment(float aValue);
	//void Move(EMovementDirection::Type aDirection);
	//void StopMove(EMovementDirection::Type aDirection);
	void Jump();

	void CutPolygon();
	//CREATE_FUNC(Player);

	void MapInput(EInputMapping::Type aType, bool aValue);

	//cocos2d::Sprite
private:
	

	
	Texture2D* mySpriteSheet;
	Sprite* mySprite;
	Vector<SpriteFrame*> mySpriteFrames;
	Animation* myLeftAnimation;
	RepeatForever* myLeftAnimationAction;
	Animation* myRightAnimation;	
	RepeatForever* myRightAnimationAction;

	Vec2 myMovementInput;
	float myMovement;	//horizontal
	Vec2 myPreviousFramePosition;
	Vec2 myMovementVector;

	std::bitset<EInputMapping::Num> myInputMapping;
	

	float myMovementAcceleration;
	float myMovmentDeacceleration;
	float myMaxMovementVelocity;

	float myDashDeacceleration;
	//EMovementDirection::Type myMovementDirection;	

	float myHalfHeight;
	bool myIsOnGround;
	
	bool myIsJumping;

	bool myIsDashing;
	Vec2 myDashStartPoint;
	Vec2 myDashEndPoint;


	Vec2 contactDrawPoint;


	//Polygon to cut
	GeoPolygon* myDashCutPolygon;
	GrowingArray<GeoPolygon*> myPolygons;
	GrowingArray<GrowingArray<GeoVertex>> myNewPolygons;


	

	

};


#endif