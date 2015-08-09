#include "Player.h"


#include "IsoMath.h"
#include "ScreenInfo.h"


bool Player::init()
{
	if (!Node::init())
	{
		return false;
	}

	myDebugDrawNode = DrawNode::create();
	addChild(myDebugDrawNode);

	
	myMovement = 0.f;
	myHalfHeight = 32.f;
	myIsJumping = false;
	myIsOnGround = false;
	myIsDashing = false;

	myMovementAcceleration = 100.f;
	myMovmentDeacceleration = 100.f;
	myMaxMovementVelocity = 15.f;

	myDashDeacceleration = 300.f;
	
	myDebugDrawNode->setPosition(Vec2(0,0));


	scheduleUpdate();

	//Input

	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = CC_CALLBACK_2(Player::OnKeyPressed, this);
	keyboardEventListener->onKeyReleased = CC_CALLBACK_2(Player::OnKeyReleased, this);

	_eventDispatcher->addEventListenerWithFixedPriority(keyboardEventListener, 1);


	//Init physics
	//setPhysicsBody(PhysicsBody::createCircle(32, PhysicsMaterial(1, 0, 1)));
	PhysicsBody* phyBody = getPhysicsBody();
	if (phyBody)
	{
		phyBody->setGravityEnable(true);
		phyBody->setAngularVelocityLimit(0.f);		
	}

	//Sprite	

	

	mySpriteSheet = Director::getInstance()->getTextureCache()->addImage("TestSpriteSheet.png");

	Vector<SpriteFrame*> leftAnimSpriteFrames;
	for (int i = 0; i < 6; i++)
	{
		leftAnimSpriteFrames.pushBack(SpriteFrame::createWithTexture(mySpriteSheet, Rect(i * 64, 0, 64, 128)));
		SpriteFrameCache::getInstance()->addSpriteFrame(leftAnimSpriteFrames.back(), "Left_" + i);
	}

	Vector<SpriteFrame*> rightAnimSpriteFrames;
	for (int i = 0; i < 6; i++)
	{
		rightAnimSpriteFrames.pushBack(SpriteFrame::createWithTexture(mySpriteSheet, Rect(i * 64, 128, 64, 128)));
		SpriteFrameCache::getInstance()->addSpriteFrame(rightAnimSpriteFrames.back(), "Right_" + i);
	}

	mySprite = Sprite::createWithSpriteFrame(leftAnimSpriteFrames.at(0));
	addChild(mySprite);
	mySprite->setVisible(false);

	myLeftAnimation = Animation::createWithSpriteFrames(leftAnimSpriteFrames, 0.15f);
	myLeftAnimationAction = RepeatForever::create(Animate::create(myLeftAnimation));
	AnimationCache::getInstance()->addAnimation(myLeftAnimation, "Left");
	
	myRightAnimation = Animation::createWithSpriteFrames(rightAnimSpriteFrames, 0.15f);
	myRightAnimationAction = RepeatForever::create(Animate::create(myRightAnimation));
	AnimationCache::getInstance()->addAnimation(myRightAnimation, "Right");

	mySprite->runAction(myRightAnimationAction);
	mySprite->runAction(myLeftAnimationAction);
	

	//Polygon
	GrowingArray<GeoVertex> verts(4);

	Vec2 leftBottom = Vec2(200, 100);
	Vec2 size = Vec2(75, 400);

	verts.Add(leftBottom);
	verts.Add(leftBottom + Vec2(0, size.y));
	verts.Add(leftBottom + Vec2(size.x, size.y));
	verts.Add(leftBottom + Vec2(size.x, 0));

	GeoPolygon* polygon = new GeoPolygon();
	polygon->Init(verts);

	myDashCutPolygon = new GeoPolygon();
	
	myPolygons.Init(4);
	myPolygons.Add(polygon);


	//
	auto ground = Node::create();

	Vec2 startPos = ScreenInfo::leftBottom() + Vec2(0, 400) + Vec2(-10000, 0);

	Vec2 points[9];
	points[0] = startPos;
	points[1] = ScreenInfo::leftBottom() + Vec2(0, 400);
	points[2] = points[1] + Vec2(200, 0);
	points[3] = points[2] + Vec2(200, -50);
	points[4] = points[3] + Vec2(200, 0);
	points[5] = points[4] + Vec2(0, 10);
	points[6] = points[5] + Vec2(100, 0);
	points[7] = points[6] + Vec2(0, 32);
	points[8] = points[7] + Vec2(10000, 0);

	ground->setPhysicsBody(PhysicsBody::createEdgeChain(points, 9));
	//ground->setPhysicsBody(PhysicsBody::createEdgeSegment(ScreenInfo::leftBottom() + Vec2(0, 100), ScreenInfo::rightBottom() + Vec2(0, 100)));

	
	


	setPosition(ScreenInfo::center());

	return true;
}

Player* Player::create()
{
	Player * ret = new (std::nothrow) Player();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void Player::update(float aDeltaTime)
{
	
	//UpdateMovementInput
	{
		myMovementInput = Vec2::ZERO;
		myMovementInput.x += myInputMapping[EInputMapping::Move_Right];
		myMovementInput.x += myInputMapping[EInputMapping::Move_Left] * -1;	

		myMovementInput.y += myInputMapping[EInputMapping::Move_Up];
		myMovementInput.y += myInputMapping[EInputMapping::Move_Down] * -1;

	}

	//UpdateMovement
	if (myIsDashing)
	{
		UpdateDashingMovement(aDeltaTime, myMovementVector);
	}
	else
	{
		UpdateNormalMovement(aDeltaTime,myMovementVector);		
	}

	Vec2 pos = getPosition();
	pos += myMovementVector;
	setPosition(pos);

	PhysicsWorld* physicsWorld = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	physicsWorld->rayCast(CC_CALLBACK_3(Player::OnRayCastIntersect, this), myPreviousFramePosition, pos, nullptr);
	physicsWorld->rayCast(CC_CALLBACK_3(Player::OnRayCastIntersect, this), pos, pos - Vec2(0, myHalfHeight), nullptr);
	

	Camera* defaultCamera = Director::getInstance()->getRunningScene()->getDefaultCamera();
	defaultCamera->setPosition(defaultCamera->getPosition().lerp(pos, aDeltaTime * 15.f));


	myPreviousFramePosition = getPosition();
}



void Player::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	myDebugDrawNode->clear();
	//myDebugDrawNode->drawCircle(getPosition(),32, getRotation(), 8, true, Color4F::GREEN);
	myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(getPosition()), myHalfHeight, getRotation(), 8, true, Color4F::WHITE);


	
	myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(ScreenInfo::left()), 32, getRotation(), 8, true, Color4F::BLUE);
	myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(ScreenInfo::right()), 32, getRotation(), 8, true, Color4F::RED);
	myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(ScreenInfo::top()), 32, getRotation(), 8, true, Color4F::MAGENTA);
	myDebugDrawNode->drawCircle(myDebugDrawNode->convertToNodeSpace(ScreenInfo::bottom()), 32, getRotation(), 8, true, Color4F::MAGENTA);


	

	Color4F col = myIsOnGround ? Color4F::RED : Color4F::GREEN;
	myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(getPosition()),
		myDebugDrawNode->convertToNodeSpace(getPosition()) - Vec2(0, myHalfHeight), col);

	myDebugDrawNode->drawPoint(myDebugDrawNode->convertToNodeSpace(contactDrawPoint), 12, col);
	

	//Input dir:
	Vec2 drawPos = Vec2(-120, 120);
	Vec2 inputDir = myMovementInput;
	float dirMagnitude = 75.f;
	myDebugDrawNode->drawCircle(drawPos, 16, 0, 8, false, Color4F::RED);
	myDebugDrawNode->drawLine(drawPos, drawPos + inputDir * dirMagnitude, Color4F::RED);


	myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(getPosition()), myDebugDrawNode->convertToNodeSpace(getPosition() + myMovementVector), Color4F::MAGENTA);

	//dashing
	myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(myDashStartPoint), myDebugDrawNode->convertToNodeSpace(myDashEndPoint), Color4F::RED);



	//Draw polygon
	for (int i = 0; i < myPolygons.Count(); i++)
	{
		DrawPolygon(*myPolygons[i],Color4F::WHITE);
	}

}
void Player::DrawPolygon(const GeoPolygon& aPoly, const Color4F& aColor)
{
	GrowingArray<GeoEdge> polyEdges(4);
	aPoly.GetEdges(polyEdges);


	GrowingArray<GeoVertex> verts;
	aPoly.GetVertices(verts);

	//	myDebugDrawNode->drawSolidPoly(verts.GetArray(), verts.Count(), aLineColor);
	for (int i = 0; i < polyEdges.Count(); ++i)
	{
		
		myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(polyEdges[i].myStart), myDebugDrawNode->convertToNodeSpace(polyEdges[i].myEnd), aColor);
		Vec2 line = (polyEdges[i].myEnd - polyEdges[i].myStart);
		Vec2 center = polyEdges[i].myStart + line / 2;
		Vec2 centrtOffSet = line.getNormalized();
		centrtOffSet.rotate(Vec2::ZERO, -(float)M_PI_2);
		centrtOffSet *= 15.f;
		myDebugDrawNode->drawPoint(myDebugDrawNode->convertToNodeSpace(center), 2, aColor);
		myDebugDrawNode->drawLine(myDebugDrawNode->convertToNodeSpace(center), myDebugDrawNode->convertToNodeSpace(center + centrtOffSet), aColor);


	}

}
void Player::OnKeyPressed(EventKeyboard::KeyCode aKeyCode, Event* aEvent)
{
	switch (aKeyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		AddMovment(-1.f);
		MapInput(EInputMapping::Move_Left, true);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		AddMovment(1.f);
		MapInput(EInputMapping::Move_Right, true);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		MapInput(EInputMapping::Move_Up, true);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		MapInput(EInputMapping::Move_Down, true);
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		Jump();
		MapInput(EInputMapping::Jump, true);
		break;
	}

}
void Player::OnKeyReleased(EventKeyboard::KeyCode aKeyCode, Event* aEvent)
{
	
	switch (aKeyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		AddMovment(1.f);
		MapInput(EInputMapping::Move_Left, false);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		AddMovment(-1.f);
		MapInput(EInputMapping::Move_Right, false);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		MapInput(EInputMapping::Move_Up, false);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		MapInput(EInputMapping::Move_Down, false);
		break;
	}
}

bool Player::OnRayCastIntersect(PhysicsWorld& aWorld, const PhysicsRayCastInfo& someInfo, void* someData)
{

	contactDrawPoint = someInfo.contact;
	setPositionY(someInfo.contact.y + myHalfHeight-1.f);
	myIsOnGround = true;
	myIsJumping = false;

	return true;
}
void Player::UpdateNormalMovement(const float aDeltaTime, Vec2& aInOutMovementVector)
{
	//Apply gravity
	UpdateGravityMovement(aDeltaTime, aInOutMovementVector);

	if (myIsOnGround)
	{
		aInOutMovementVector.y = 0.f;
	}
	
		//Ground movmeent update

		if (myMovement == 0.f)
		{
			if (aInOutMovementVector.x != 0.f)
			{
				//Deaccelerate
				float oppositeDirection = (aInOutMovementVector.x / abs(aInOutMovementVector.x)) * -1.f;

				float maxDeaccelerationDelta = abs(aInOutMovementVector.x);

				aInOutMovementVector.x += oppositeDirection * min(aDeltaTime * myMovmentDeacceleration, maxDeaccelerationDelta);
			}
		}
		else
		{
			//Accelrate
			aInOutMovementVector.x += myMovement * aDeltaTime * myMovementAcceleration;
		}

		
		aInOutMovementVector.x = clampf(aInOutMovementVector.x, myMaxMovementVelocity * -1, myMaxMovementVelocity);
	

	myIsOnGround = false;
	
}
void Player::UpdateDashingMovement(const float aDeltaTime, Vec2& aInOutMovementVector)
{
 	Vec2 dashDir = (myDashEndPoint - myDashStartPoint).getNormalized();
	
	//Tests if player is past the end dash point;
	float dotTest = (myDashEndPoint - getPosition()).dot(dashDir);
	if (dotTest <= 0)
	{
		//StopDash;

		//Deaccelerate until normal speed
		UpdateGravityMovement(aDeltaTime, aInOutMovementVector);
		if (myIsOnGround)
		{
			aInOutMovementVector.y = 0.f;
		}

		//Dash deacc
		Vec2 oppositeDir = dashDir * -1.f;//aInOutMovementVector.getNormalized() * -1.f;

		//float maxDeaccelerationDelta = aInOutMovementVector abs(aInOutMovementVector.x);

		aInOutMovementVector += oppositeDir * aDeltaTime * myDashDeacceleration;//min(aDeltaTime * myDashDeacceleration, maxDeaccelerationDelta);

		if (aInOutMovementVector.x <= myMaxMovementVelocity)
		{
			myIsDashing = false;
		}	
		
	}
	else
	{
		float dashAcceleration = 800.f;
		aInOutMovementVector += dashDir * dashAcceleration * aDeltaTime;
	}
	
	
}
void Player::UpdateGravityMovement(const float aDeltaTime, Vec2& aInOutMovementVector)
{
	aInOutMovementVector.y -= aDeltaTime * 100.f;
}
void Player::AddMovment(float aValue)
{
	myMovement += aValue;

	if (abs(myMovement) > 1.f)
	{
		bool breakasdsa = false;
	}

	if (myMovement > 0.f)
	{
		mySprite->stopAllActions();
		//mySprite->runAction(RepeatForever::create(Animate::create(myRightAnimation)));
		mySprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("Right"))));
	}
	else if (myMovement < 0.f)
	{
		mySprite->stopAllActions();
		//mySprite->runAction(RepeatForever::create(Animate::create(myLeftAnimation)));
		mySprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("Left"))));
	}
	else
	{
		mySprite->stopAllActions();
	}
}

//void Player::Move(EMovementDirection::Type aDirection)
//{
//	if (myMovementDirection != aDirection)
//	{
//
//	}
//	switch (aDirection)
//	{
//	case EMovementDirection::Left:
//		myMovement = -1.f;
//		break;
//	case EMovementDirection::Right:
//		myMovement = 1.f;
//		break;	
//	}
//
//	myMovementDirection = aDirection;
//}
//void Player::StopMove(EMovementDirection::Type aDirection)
//{
//	if (myMovement == aDirection)
//	{
//		myMovement = 
//	}
//}
void Player::Jump()
{	
	if (myIsJumping )
	{
		if (!myIsDashing)
		{
			if (myMovementInput.isZero() == false)
			{
				myIsDashing = true;
				myDashStartPoint = getPosition();
				float dashLength = 150.f;
				myDashEndPoint = myDashStartPoint + myMovementInput * dashLength;

				Vec2 dashDir = (myDashEndPoint - myDashStartPoint).getNormalized();
				Vec2 dashTan = dashDir.rotateByAngle(Vec2::ZERO, (float)M_PI_2);
				GrowingArray<GeoVertex> newDashCutPoly(2);

				float dashLineSize = 2.f;
				Vec2 dashLineOffset = dashTan * (dashLineSize / 2.f);

				newDashCutPoly.Add(myDashStartPoint - dashLineOffset);
				newDashCutPoly.Add(myDashStartPoint + dashLineOffset);
				newDashCutPoly.Add(myDashEndPoint + dashLineOffset);
				newDashCutPoly.Add(myDashEndPoint - dashLineOffset);

				myDashCutPolygon->Init(newDashCutPoly);

				CutPolygon();
				//myMovementVector = myMovementVector.length() * dashDir;
			}
		}
	}
	else
	{
		myIsJumping = true;
		myIsOnGround = false;
		myMovementVector.y += 20.f;
	}
	
}

void Player::CutPolygon()
{
	int numPolys = myPolygons.Count();
	for (int i = numPolys - 1; i > -1; i--)
	{
		bool clipping = GeoPolygonOperations::WeilerAthertonClipping(*myPolygons[i],*myDashCutPolygon, myNewPolygons, EClippingMode::AnotB);

		if (clipping && myNewPolygons.Count() > 0)
		{
			//Replace with first poly
			myPolygons[i]->Init(myNewPolygons[0]);
			myPolygons[i]->SetPositionToCenterOfVerices();

			//If more polys, add them
			if (myNewPolygons.Count() > 1)
			{
				for (int i = 1; i < myNewPolygons.Count(); ++i)
				{
					myPolygons.Add(new GeoPolygon());
					myPolygons.GetLast()->Init(myNewPolygons[i]);

					Color4F color;
					color.r = rand_0_1();
					color.g = rand_0_1();
					color.b = rand_0_1();

					color.r = (color.r + Color4F::WHITE.r) / 2.f;
					color.g = (color.g + Color4F::WHITE.g) / 2.f;
					color.b = (color.b + Color4F::WHITE.b) / 2.f;
					color.a = 1.f;
					//myPolyColors.Add(color);
				}
			}
		}
	}
}

void Player::MapInput(EInputMapping::Type aType, bool aValue)
{
	myInputMapping.set(aType, aValue);
}