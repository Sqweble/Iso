
#include "InputScene.h"
#include "ScreenInfo.h"

#include "Polygon.h"
#include "PolygonOperations.h"





Scene* InputScene::createScene()
{
	auto scene = Scene::create();
	auto inputLayer = InputScene::create();
	

	scene->addChild(inputLayer);
	

	return scene;
}
bool InputScene::init()
{
	if (!Layer::init())
		return false;


	myRayCastInfo.Init(4);
	myPointCloud.Init(4);
	myRandomPointsIn.Init(4);
	myRandomPointsOut.Init(4);

	myPhysicsLayer = Scene::create();
	addChild(myPhysicsLayer);
	
	

	myLabelTouchInfo = Label::createWithSystemFont("", "Arial", 30);
	//myLabelTouchInfo->setPosition(Vec2(
	//	Director::getInstance()->getVisibleSize().width / 2,
	//	Director::getInstance()->getVisibleSize().height / 2));

	addChild(myLabelTouchInfo);

	

	auto touchListener = EventListenerTouchOneByOne::create();

	//touchListener->onTouchBegan		= CC_CALLBACK_2(InputScene::onTouchBegan		, this);
	touchListener->onTouchEnded		= CC_CALLBACK_2(InputScene::onTouchEnded		, this);
	touchListener->onTouchMoved		= CC_CALLBACK_2(InputScene::onTouchMoved		, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(InputScene::onTouchCancelled	, this);
	
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(InputScene::OnMouseDown, this);

	auto sprite = Sprite::create("HelloWorld.png");
	sprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
	Director::getInstance()->getVisibleSize().height / 2));
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouseListener, 1);

	//myPhysicsLayer->addChild(sprite);


	myDebugDrawNode = DrawNode::create();
	addChild(myDebugDrawNode);







	//auto ground = Node::create();
	//ground->setPhysicsBody(PhysicsBody::createEdgeSegment(ScreenInfo::leftBottom() + Vec2(0, 50), ScreenInfo::rightBottom() + Vec2(0, 50)));
	//addChild(ground);

	
	//
	auto box = Node::create();
	Vec2 points[4] = { Vec2(-100, -100), Vec2(-100, 100), Vec2(100, 100), Vec2(100, -100) };
	//box->setPhysicsBody(PhysicsBody::createEdgePolygon(points, 4));
	box->setPhysicsBody(PhysicsBody::createPolygon(points, 4));
	box->setPosition(ScreenInfo::center());		
//	box->getPhysicsBody()->
	//box->getPhysicsBody()->setTag(_sliceTag);
	//addChild(box);


	//Make pattern
	myPattern.CreateNewVertex(myRootPatterVertex);

	myRootPatterVertex->SetPosition(ScreenInfo::center());
	
	

	PatternVertex* curParrent = myRootPatterVertex;	
	PatternEdge* edge;

	PatternVertex* aVert;
	myPattern.CreateNewVertex(aVert, edge, myRootPatterVertex);
	aVert->SetPosition(ScreenInfo::center() + Vec2(-100, -100));
	
	PatternVertex* bVert;
	myPattern.CreateNewVertex(bVert, edge, myRootPatterVertex);
	bVert->SetPosition(ScreenInfo::center() + Vec2(100, -100));

	PatternVertex* cVert;
	myPattern.CreateNewVertex(cVert, edge, myRootPatterVertex);
	cVert->SetPosition(ScreenInfo::center() + Vec2(100, 100));


	GrowingArray<GeoVertex> verts(4);
	float size = 50.f;
	verts.Add(Vec2(-size, size));
	verts.Add(Vec2(size, size));

	verts.Add(Vec2(size, size * 0.5));
	verts.Add(Vec2(0, size * 0.5));
	verts.Add(Vec2(0, -size * 0.5));
	verts.Add(Vec2(size, -size * 0.5));	

	verts.Add(Vec2(size, -size));
	verts.Add(Vec2(-size, -size));

	myMousePolygon = GeoPolygon::create();
	myMousePolygon->SetWorldVertices(verts);

	for (int i = 0; i < verts.Count(); ++i)
	{
		verts[i] *= 5;
	}

	myPolygon = GeoPolygon::create();
	myPolygon->SetLocalVertices(verts, ScreenInfo::center());
	

	myPolygons.Init(4);
	myPolygons.Add(myPolygon);
	myPolyColors.Init(4);
	myPolyColors.Add(Color4F::WHITE);

	verts.Insert(2, Vec2(150, 0));

	

	myAandB = new GeoPolygon();
	myAnotB = new GeoPolygon();

	GrowingArray<GeoEdge> edges(4);
	myPolygon->GetEdgesLocal(edges);



	for (int i = 0; i < 500; i++)
	{
		float randX = (rand_0_1() - 0.5f) * 2;
		float randY = (rand_0_1() - 0.5f) * 2;

		Vec2 randPoint = (Vec2(randX, randY) * 400.f) + ScreenInfo::center();

		if (GeoPolygonOperations::Inside(randPoint, *myPolygon))
		{
			myRandomPointsIn.Add(randPoint);
		}
		else
		{
			myRandomPointsOut.Add(randPoint);
		}


	}
	
	

	return true;
	
}



void InputScene::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	myDebugDrawNode->clear();

	Layer::draw(renderer, transform, flags);	
	myDebugDrawNode->drawPoint(myFirstRayPoint, 8.f, Color4F::GREEN);
	myDebugDrawNode->drawPoint(mySecondRayPoint, 8.f, Color4F::BLUE);
	myDebugDrawNode->drawLine(myFirstRayPoint, mySecondRayPoint, Color4F::WHITE);
		

	myDebugDrawNode->drawPoint(myRayInPoint, 8.f, Color4F::GREEN);
	myDebugDrawNode->drawPoint(myRayOutPoint, 8.f, Color4F::BLUE);


	//POintCloud
	Vec2 rayNorm = mySecondRayPoint - myFirstRayPoint;
	rayNorm.normalize();
	float rayAngle = rayNorm.getAngle(Vec2(0, 1));

	Vec2 orgin = myFirstRayPoint;
	Vec2 scale(1.f, 1.f);
	
	scale *= (mySecondRayPoint - myFirstRayPoint).length();
	
	//for (int i = 0; (unsigned int)i < myPointCloud.Count(); ++i)
	//{
	//	Vec2 point = myPointCloud[i];
	//			
	//	point.scale(scale);		
	//	point.rotate(rayNorm);		
	//	point += orgin;
	//
	//	myDebugDrawNode->drawLine(point, point + 8.f * rayNorm, Color4F::ORANGE);
	//}
			
	//float depth = 0.f;
	//myRootPatterVertex->DrawHierarchly(myDebugDrawNode, depth);

	for (int i = 0; i < myPolygons.Count(); ++i)
	{
		//DrawPolygon(*myPolygons[i], myPolyColors[i], Color4F::WHITE, Color4F::WHITE, Color4F::WHITE);
	}

	//DrawPolygon(*myPolygon, Color4F::WHITE, Color4F::WHITE, Color4F::GRAY, Color4F::GRAY);	
	//DrawPolygon(*myMousePolygon, Color4F::GREEN, Color4F::GREEN, Color4F::RED, Color4F::WHITE);
	
	//DrawPolygon(*myAandB, Color4F::ORANGE, Color4F::ORANGE, Color4F::GREEN, Color4F::RED);
	//DrawPolygon(*myAnotB, Color4F::GREEN, Color4F::GREEN, Color4F::GREEN, Color4F::RED);
	

	GrowingArray<GeoEdge> aAndBEdges(4);
	
	for (int i = 0; i < aAndBEdges.Count(); ++i)
	{
		myDebugDrawNode->drawLine(aAndBEdges[i].myStart, aAndBEdges[i].myEnd, Color4F::ORANGE);
	}

	for (int i = 0; i < myRandomPointsIn.Count(); ++i)
	{
		myDebugDrawNode->drawPoint(myRandomPointsIn[i], 4.f, Color4F::RED);
	}
	for (int i = 0; i < myRandomPointsOut.Count(); ++i)
	{
		myDebugDrawNode->drawPoint(myRandomPointsOut[i], 4.f, Color4F::WHITE);
	}

	
	for (int i = 0; i < myPolyVertIndicesLabels.Count(); ++i)
	{
		for (int j = 0; j < myPolyVertIndicesLabels[i].Count(); ++j)
		{
			myPolyVertIndicesLabels.At(i).At(j)->draw(renderer, transform, flags);
		}
		
	}
}

void InputScene::OnMouseDown(cocos2d::Event* aEvent)
{
	EventMouse* mouseEvent = reinterpret_cast<EventMouse*>(aEvent);
	if (mouseEvent)
	{
		int button = mouseEvent->getMouseButton();
		Vec2 location = mouseEvent->getLocationInView();
		location.y += abs(ScreenInfo::top().y - ScreenInfo::bottom().y);
		
		int numPolys = myPolygons.Count();
		for (int i = numPolys - 1; i > -1; i--)
		{
			bool clipping = GeoPolygonOperations::WeilerAthertonClipping(*myPolygons[i], *myMousePolygon, myNewPolygons, EClippingMode::AnotB);

			if (clipping && myNewPolygons.Count() > 0)
			{
				//Replace with first poly
				myPolygons[i]->SetWorldVertices(myNewPolygons[0]);
				

				//If more polys, add them
				if (myNewPolygons.Count() > 1)
				{
					for (int i = 1; i < myNewPolygons.Count(); ++i)
					{
						myPolygons.Add(new GeoPolygon());
						myPolygons.GetLast()->SetWorldVertices(myNewPolygons[i]);

						Color4F color;
						color.r = rand_0_1();
						color.g = rand_0_1();
						color.b = rand_0_1();

						color.r = (color.r + Color4F::WHITE.r) / 2.f;
						color.g = (color.g + Color4F::WHITE.g) / 2.f;
						color.b = (color.b + Color4F::WHITE.b) / 2.f;
						color.a = 1.f;
						myPolyColors.Add(color);
					}
				}
			}

		}

		myMousePolygon->setPosition(location);
		switch (button)
		{
		case 0:
			
			break;
		}
	}
	
	
}

bool InputScene::onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{	
	
	return true;
}
void InputScene::onTouchEnded(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
	//myLabelTouchInfo->setPosition(aTouch->getLocation());
	//myLabelTouchInfo->setString("onTouchEnded");

	myMousePolygon->setPosition(aTouch->getLocation());
	myAandB->setPosition(Vec2(0, 0));//aTouch->getLocation();
	myAnotB->setPosition(Vec2(0, 0));
	

	//GeoPolygonOperations::AandB(*myPolygon, *myMousePolygon, *myAandB);
	

	//GeoPolygonOperations::WeilerAthertonClipping(*myPolygon, *myMousePolygon, myAandB->myVertices,EClippingMode::AandB);

	
	int numPolys = myPolygons.Count();
	for (int i = numPolys-1; i > -1; i--)
	{
		bool clipping = GeoPolygonOperations::WeilerAthertonClipping(*myPolygons[i], *myMousePolygon, myNewPolygons, EClippingMode::AnotB);

		if (clipping && myNewPolygons.Count() > 0)
		{
			//Replace with first poly
			myPolygons[i]->SetWorldVertices(myNewPolygons[0]);
			

			//If more polys, add them
			if (myNewPolygons.Count() > 1)
			{
				for (int i = 1; i < myNewPolygons.Count(); ++i)
				{
					myPolygons.Add(new GeoPolygon());
					myPolygons.GetLast()->SetWorldVertices(myNewPolygons[i]);

					Color4F color;
					color.r = rand_0_1();
					color.g = rand_0_1();
					color.b = rand_0_1();

					color.r = (color.r + Color4F::WHITE.r) / 2.f;
					color.g = (color.g + Color4F::WHITE.g) / 2.f;
					color.b = (color.b + Color4F::WHITE.b) / 2.f;
					color.a = 1.f;
					myPolyColors.Add(color);
				}
			}
		}

	}

	
	for (int i = 0; i < myPolyVertIndicesLabels.Count(); ++i)
	{
		for (int j = 0; j < myPolyVertIndicesLabels.At(i).Count(); ++j)
		{
			removeChild(myPolyVertIndicesLabels.At(i).At(j));
			//myPolyVertIndicesLabels.At(i).At(j)->release();
		}
	}
	myPolyVertIndicesLabels.RemoveAll();
	myPolyVertIndicesLabels.Init(32);

	for (int i = 0; i < myPolygons.Count(); ++i)
	{
		myPolyVertIndicesLabels.Add(GrowingArray<cocos2d::Label*>(4));
		GrowingArray<cocos2d::Label*>& labels = myPolyVertIndicesLabels.GetLast();

		for (int j = 0; j < myPolygons[i]->GetVerticesNum(); ++j)
		{
			std::string vertIndex = std::to_string(j);			
			
			
			labels.Add(Label::createWithSystemFont(vertIndex, "Arial", 12));
			//labels.GetLast()->setPosition(myPolygons[i]->GetVertices()[j] + myPolygons[i]->GetPosition() + Vec2(8,8));
			addChild(labels.GetLast());
		}
	}

	myPolyVertIndicesLabels.Add(GrowingArray<cocos2d::Label*>(4));
	GrowingArray<cocos2d::Label*>& labels = myPolyVertIndicesLabels.GetLast();
	for (int i = 0; i < myMousePolygon->GetVerticesNum(); ++i)
	{
		

		labels.Add(Label::createWithSystemFont(std::to_string(i), "Arial", 12));
//		labels.GetLast()->setPosition(myMousePolygon->GetVertices()[i] + myMousePolygon->GetPosition() + Vec2(8, 8));
		addChild(labels.GetLast());
	}

	/*bool r = GeoPolygonOperations::WeilerAthertonClipping(*myPolygon, *myMousePolygon, myNewPolygons, EClippingMode::AnotB);
	if (r)
	{
	int countDiff = myNewPolygons.Count() - myPolygons.Count();
	for (int i = 0; i < myNewPolygons.Count(); ++i)
	{

	}
	myPolygon->Init(myAnotB->myVertices);
	myPolygon->SetPositionToCenterOfVerices();
	}*/
	



	UpdateRayInfo();
}
void InputScene::onTouchMoved(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
	//myLabelTouchInfo->setPosition(aTouch->getLocation());
	//myLabelTouchInfo->setString("onTouchMoved");
	mySecondRayPoint = aTouch->getLocation();

//	myMousePolygon->SetPosition(aTouch->getLocation());

	UpdateRayInfo();
}
void InputScene::onTouchCancelled(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
	//myLabelTouchInfo->setPosition(aTouch->getLocation());
	//myLabelTouchInfo->setString("onTouchCancelled");
}

void InputScene::UpdateRayInfo()
{
	myRayCastInfo.RemoveAll();
	PhysicsRayCastCallbackFunc rayCastCallback = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		myRayCastInfo.Add(info);
		//myRayCastInfo.Add(info);
		return true;
	};

	
	CCDirector::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCastCallback, myFirstRayPoint, mySecondRayPoint, nullptr);

	if (myRayCastInfo.Count() > 0)
	{
		myRayInPoint = myRayCastInfo[0].contact;

		Vec2 end = myRayInPoint;
		Vec2 start = mySecondRayPoint;
		if (myRayCastInfo.Count() > 1)
		{
			start = myRayCastInfo[1].contact;
		}		
		myRayCastInfo.RemoveAll();
		CCDirector::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(rayCastCallback, start, end, nullptr);
		if (myRayCastInfo.Count() > 0)
		{
			myRayOutPoint = myRayCastInfo[0].contact;
		}
	}
}

void InputScene::GenerateTreePattern()
{
	int numPoints = 20;

	GrowingArray<Vec2> pointCloud(4);

	myPointCloud.RemoveAll();

	float p = (1 - 0.5f) * 2;
	float m = (-0.5f) * 2;
	myPointCloud.Add(Vec2(p, p));
	myPointCloud.Add(Vec2(p, m));
	myPointCloud.Add(Vec2(m, p));
	myPointCloud.Add(Vec2(m, m));


	for (int i = 0; i < numPoints; ++i)
	{
		float x = (cocos2d::rand_0_1() - 0.5f) * 2;
		float y = (cocos2d::rand_0_1() - 0.5f) * 2;

		pointCloud.Add(Vec2(x, y));		
		myPointCloud.Add(Vec2(x, y));
	}
}



