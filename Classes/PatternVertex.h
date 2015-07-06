#pragma once


#include "cocos2d.h"
#include "base\CCRef.h"

USING_NS_CC;

class PatternEdge;

class PatternVertex : public Ref
{
protected:
	PatternVertex();
	virtual ~PatternVertex();

public:
	static PatternVertex* Create();
	

	void SetPosition(const Vec2& aPosition);
	const Vec2 GetPosition() const;

	bool ConnectEdge(PatternEdge* aEdge);

	void DrawHierarchly(DrawNode* aDrawNode, float& aDepth);


private:
	std::vector<PatternEdge*> myEdges;
	Vec2 myPosition;

	
};
