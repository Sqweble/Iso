#include "PolygonOperations.h"

#include "Polygon.h"

#include "StaticArray.h"

#include "FixedArray.h"






bool GeoPolygonOperations::Inside(const Vec2& aPoint, const Vec2& aStart, const Vec2& aEnd)
{
	Vec2 tang = (aEnd - aStart);
	tang.rotate(Vec2::ZERO, (float)(-M_PI_2));
	Vec2 toPoint = aStart - aPoint;

	return Vec2::dot(toPoint, tang) < 0;
}

bool GeoPolygonOperations::Inside(const Vec2& aPoint, const GrowingArray<GeoEdge>& aEdgeSegment)
{
	for (int i = 0; i < aEdgeSegment.Count(); i++)
	{
		if (!Inside(aPoint, aEdgeSegment[i].myStart, aEdgeSegment[i].myEnd))
		{
			return false;
		}
	}

	return true;
}

bool GeoPolygonOperations::Inside(const Vec2& aPoint, const GeoPolygon& aPolygon)
{
	GrowingArray<GeoEdge> edges;
	aPolygon.GetEdges(edges);
	
	return Inside(aPoint, edges);
}

void GeoPolygonOperations::AandB(const GeoPolygon& aPolyA, const GeoPolygon& aPolyB, GeoPolygon& aOutPoly)
{	
	GrowingArray<GeoEdge> polyAEdges;
	aPolyA.GetEdges(polyAEdges);

	GrowingArray<GeoEdge> polyBEdges;
	aPolyB.GetEdges(polyBEdges);

	//
	//
	//auto insertSorted = [](GeoPolygon& poly, Vec2 point)
	//{
	//	
	//	float l = point.lengthSquared();
	//	auto itPos = poly.myVertices.end();
	//	for (int i = poly.myVertices.size() - 1; i > -1; --i)
	//	{
	//		if (l > poly.myVertices[i].lengthSquared())
	//		{
	//			break;
	//		}
	//		--itPos;
	//	}

	//	poly.myVertices.insert(itPos, point);
	//};	
	//
	//
	//for (unsigned int a = 0; a < polyAEdges.size(); a++)
	//{
	//	GeoEdge& aEdge = polyAEdges[a];
	//	for (unsigned int b = 0; b < polyBEdges.size(); b++)
	//	{
	//		GeoEdge& bEdge = polyBEdges[b];
	//		if (Vec2::isSegmentIntersect(aEdge.myStart, aEdge.myEnd, bEdge.myStart, bEdge.myEnd))
	//		{


	//			Vec2 intersect = Vec2::getIntersectPoint(aEdge.myStart, aEdge.myEnd, bEdge.myStart, bEdge.myEnd);
	//			//aOutPoly.myVertices.push_back(intersect);
	//			insertSorted(aOutPoly, intersect);				
	//		}		
	//	}
	//}

	//for (int i = 0; i < aPolyA.myVertices.size(); ++i)
	//{
	//	Vec2 loc = aPolyA.myPosition + aPolyA.myVertices[i];
	//	if (Inside(loc, polyBEdges))
	//		insertSorted(aOutPoly, loc);
	//}

	//for (int i = 0; i < aPolyB.myVertices.size(); ++i)
	//{
	//	Vec2 loc = aPolyB.myPosition + aPolyB.myVertices[i];
	//	if (Inside(loc, polyAEdges))
	//		insertSorted(aOutPoly, loc);
	//}
	//

	//aOutPoly.FixHull();


	
	
}


void GeoPolygonOperations::AnotB(const GeoPolygon& aPolyA, const GeoPolygon& aPolyB, GeoPolygon& aOutPoly)
{

}






void PrepareForWeilerAthertonClipping(GrowingArray<GeoVertexClipNode*>* inOutSubjectNodes, GrowingArray<GeoVertexClipNode*>* inOutClipNodes, FixedArray<GeoVertexClipNode>* aIntersectionNodesContainer)
{

	GrowingArray<GeoVertexClipNode*>& newSubjectNodes = *inOutSubjectNodes;
	GrowingArray<GeoVertexClipNode*>& newClipNodes = *inOutClipNodes;

	FixedArray<GeoVertexClipNode>& intersectionNodes = *aIntersectionNodesContainer;

	int sCount = newSubjectNodes.Count();
	int cCount = newClipNodes.Count();


	Vec2 s0, s1;
	Vec2 c0, c1;

	GrowingArray<GeoVertexClipNodeIntersectionSortData> intersectionsInOrder(2);	//index & length from lineStart to intersectionpoint (sorted)



	for (int c = 0; c < newClipNodes.Count(); ++c)
	{
		c0 = /*cPos +*/ newClipNodes[c]->myVertex;
		c1 = /*cPos +*/ newClipNodes[(c + 1) % cCount]->myVertex;

		for (int s = 0; s < newSubjectNodes.Count(); ++s)
		{
			s0 = /*sPos+*/ newSubjectNodes[s]->myVertex;
			s1 = /*sPos+*/ newSubjectNodes[(s + 1) % sCount]->myVertex;

			if (Vec2::isSegmentIntersect(s0, s1, c0, c1))
			{

				Vec2 intersectPoint = Vec2::getIntersectPoint(s0, s1, c0, c1);
				{
					if (intersectPoint == s0 || intersectPoint == s1 || intersectPoint == c0 || intersectPoint == c1)
					{
						continue;
					}
				}

				GeoVertexClipNode intersectNode(intersectPoint);

				bool outIn = (s1 - s0).cross((c1 - c0)) < 0;
				if (outIn)
					intersectNode.myClipType = EClippingIntersectionType::OutIn;
				else
					intersectNode.myClipType = EClippingIntersectionType::InOut;

				intersectNode.myIsIntersectionPoint = true;
				intersectNode.myVertex = intersectPoint;

				//add intersection sorted
				float l = (intersectPoint - c0).getLengthSq();				

				GeoVertexClipNodeIntersectionSortData sortData;
				sortData.myIndex = s;
				sortData.myLengthFromLineStart = l;
				sortData.myNode = intersectNode;
				

				int numIndices = intersectionsInOrder.Count();
				int insertIndex = 0;
				if (numIndices > 0)
				{
					for (int i = 0; i < numIndices; ++i)
					{
						if (intersectionsInOrder[i].myLengthFromLineStart > l)													
							break;						
						insertIndex++;
					}					
				}

				intersectionsInOrder.Insert(insertIndex, sortData);				
			}
		}

		//Add new sorted intersections
		int numNewIntersections = intersectionsInOrder.Count();		
		int prevSubjectIndex = -1;
		for (int i = 0; i < numNewIntersections; ++i)
		{
 			
			intersectionNodes.Add(intersectionsInOrder[i].myNode);

			int subjectIndex = intersectionsInOrder[i].myIndex;

			if (subjectIndex < prevSubjectIndex)			
				newSubjectNodes.Insert(subjectIndex + 1, &intersectionNodes[intersectionNodes.Count() - 1]);
			else
				newSubjectNodes.Insert(subjectIndex + i + 1, &intersectionNodes[intersectionNodes.Count() - 1]);
			
			prevSubjectIndex = subjectIndex;
			
			newClipNodes.Insert(c + 1, &intersectionNodes[intersectionNodes.Count() - 1]);
			sCount++;
			cCount++;
			c++;
		}
		intersectionsInOrder.RemoveAll();

	}

	EClippingLoopDirection::Type forward = EClippingLoopDirection::Forward;
	EClippingLoopDirection::Type backward = EClippingLoopDirection::Backward;


	newClipNodes[0]->myClippingLoops[EClippingLoop::Clip][backward] = newClipNodes.GetLast();
	newClipNodes[0]->myClippingLoops[EClippingLoop::Clip][forward] = newClipNodes[1];
	for (int i = 1; i < cCount; ++i)
	{
		newClipNodes[i]->myClippingLoops[EClippingLoop::Clip][backward] = newClipNodes[i - 1];
		newClipNodes[i]->myClippingLoops[EClippingLoop::Clip][forward] = newClipNodes[(i + 1) % cCount];
	}

	newSubjectNodes[0]->myClippingLoops[EClippingLoop::Subject][backward] = newSubjectNodes.GetLast();
	newSubjectNodes[0]->myClippingLoops[EClippingLoop::Subject][forward] = newSubjectNodes[1];
	for (int i = 1; i < sCount; ++i)
	{
		newSubjectNodes[i]->myClippingLoops[EClippingLoop::Subject][backward] = newSubjectNodes[i - 1];
		newSubjectNodes[i]->myClippingLoops[EClippingLoop::Subject][forward] = newSubjectNodes[(i + 1) % sCount];
	}
}

bool GeoPolygonOperations::WeilerAthertonClipping(GeoPolygon& aSubjectPolygon, GeoPolygon& aClipPolygon, GrowingArray<GrowingArray<GeoVertex>>& outNewPolys, EClippingMode::Type aClippingMode)
{	
	//Create node lists - look at how WeilerAthertonClipping works, Google is your friend

	GrowingArray<GeoVertex> subjectVerts;
	aSubjectPolygon.GetVertices(subjectVerts);

	GrowingArray<GeoVertex> clipVerts;
	aClipPolygon.GetVertices(clipVerts);

	int sCount = subjectVerts.Count();
	int cCount = clipVerts.Count();

	Vec2 sPos = aSubjectPolygon.GetPosition();
	Vec2 cPos = aClipPolygon.GetPosition();

	FixedArray<GeoVertexClipNode> subjectNodes(sCount * 2);
	GrowingArray<GeoVertexClipNode*> newSubjectNodes(32);

	FixedArray<GeoVertexClipNode> clipNodes(cCount * 2);	
	GrowingArray<GeoVertexClipNode*> newClipNodes(32);

	FixedArray<GeoVertexClipNode> intersectionNodes(32);


	
	for (int i = 0; i < sCount; ++i)
	{
		subjectNodes.Add(subjectVerts[i]);
		newSubjectNodes.Add(&subjectNodes[i]);		
	}
	for (int i = 0; i < cCount; ++i)
	{
		clipNodes.Add(clipVerts[i]);
		newClipNodes.Add(&clipNodes[i]);		
	}


	PrepareForWeilerAthertonClipping(&newSubjectNodes, &newClipNodes, &intersectionNodes);

	int newSCount = newSubjectNodes.Count();
	int newCCount = newClipNodes.Count();

	if (newSCount == sCount && newCCount == cCount)
	{
		//No intersection
		return false;
	}
	outNewPolys.RemoveAll();
	outNewPolys.Init(2);

	static int second = 0;
	//second++;
	if (second >= 2)
	{
		outNewPolys.Add(GrowingArray<GeoVertex>(2));
		GrowingArray<GeoVertex>& lol = outNewPolys.GetLast();
		for (int i = 0; i < newSCount; ++i)
		{
			lol.Add(newSubjectNodes[i]->myVertex);
		}
		return true;
	}	
	


	EClippingIntersectionType::Type inOutOrOutIn = EClippingIntersectionType::None;
	bool switchLoopDirOnIntersection = false;

	switch (aClippingMode)
	{
	case EClippingMode::AandB:
		inOutOrOutIn = EClippingIntersectionType::InOut;
		switchLoopDirOnIntersection = false;
		break;
	case EClippingMode::AnotB:
		inOutOrOutIn = EClippingIntersectionType::OutIn;
		switchLoopDirOnIntersection = true;
		break;
	default:
		break;	
	}
	

	for (int s = 0; s < newSCount; ++s)
	{
		//Find first outin

		
		if (newSubjectNodes[s]->myIsIntersectionPoint && newSubjectNodes[s]->myClipType == inOutOrOutIn && !newSubjectNodes[s]->myIsVisited)
		{
			outNewPolys.Add(GrowingArray<GeoVertex>(8));
			GrowingArray<GeoVertex>& newPoly = outNewPolys.GetLast();

			GeoVertexClipNode* startNode = nullptr;
			GeoVertexClipNode* curNode = nullptr;

			startNode = newSubjectNodes[s];
			int listIndex = EClippingLoop::Subject;						
			int loopDirection = EClippingLoopDirection::Forward;

			Color4F color;
			color = Color4F::WHITE;
			
			curNode = newSubjectNodes[s];			
			newPoly.Add(curNode->myVertex);

			curNode = curNode->myClippingLoops[listIndex][loopDirection];
			
			while (curNode != startNode)
			{	
				color.g -= 0.1f;				
				newPoly.Add(curNode->myVertex);

				if (curNode->myIsIntersectionPoint)
				{
					listIndex = !listIndex;
					if (switchLoopDirOnIntersection)
						loopDirection = !loopDirection;
					
				}		
				curNode->myIsVisited = true;
				curNode = curNode->myClippingLoops[listIndex][loopDirection];
			}			
			
		}
		
	}
	
	return true;


}

