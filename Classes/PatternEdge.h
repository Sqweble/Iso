#pragma once


#include "cocos2d.h"
#include "base\CCRef.h"



class PatternVertex;

USING_NS_CC;

class PatternEdge : public Ref
{

protected:
	PatternEdge();
	virtual ~PatternEdge();
public:	
	static PatternEdge* Create();

	bool CanConnect();
	bool IsConnectedWithVertex(PatternVertex* aVertex);

	bool ConnectVertices(PatternVertex* aFirstVert, PatternVertex* aSecondVert);
	void ConnectVertex(PatternVertex* aVertex);
	void DisconnectVertex(PatternVertex* aVertex);

	PatternVertex* GetConnectedVertex(PatternVertex* aVertex);

	void DrawHierarchly(DrawNode* aDrawNode,float& Depth);



private:

	std::pair<PatternVertex*, PatternVertex*> myVertices;

};