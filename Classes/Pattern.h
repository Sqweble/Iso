#pragma once

#include "cocos2d.h"

class PatternEdge;
class PatternVertex;

class Pattern
{
public:
	Pattern();
	~Pattern();

	void CreateNewVertex(PatternVertex*& aOutVertex);
	void CreateNewVertex(PatternVertex*& aOutVertex, PatternEdge*& aOutEdge, PatternVertex* aVertexToConnectTo);

private:
	PatternEdge* CreateEdge();
	PatternVertex* CreateVertex();
	
	
	std::vector<PatternEdge*> myEdges;
	std::vector<PatternVertex*> myVertices;


};
