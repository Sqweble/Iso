#include "Polygon.h"




// isLeft(): test if a point is Left|On|Right of an infinite 2D line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 to P1
//          =0 for P2 on the line
//          <0 for P2 right of the line
inline int
isLeft(Vec2 P0, Vec2 P1, Vec2 P2)
{
	return ((P1.x - P0.x) * (P2.y - P0.y)
		- (P2.x - P0.x) * (P1.y - P0.y));
}

// chainHull_2D(): Andrew's monotone chain 2D convex hull algorithm
//     Input:  P[] = an array of 2D points 
//                  presorted by increasing x and y-coordinates
//             n =  the number of points in P[]
//     Output: H[] = an array of the convex hull vertices (max is n)
//     Return: the number of points in H[]
int
chainHull_2D(const GrowingArray<GeoVertex>& P, int n, GrowingArray<GeoVertex>& H)
{
	// the output array H[] will be used as the stack
	int    bot = 0, top = (-1);   // indices for bottom and top of the stack
	int    i;                 // array scan index

	// Get the indices of points with min x-coord and min|max y-coord
	int minmin = 0, minmax;
	float xmin = P[0].x;
	for (i = 1; i < n; i++)
		if (P[i].x != xmin) break;
	minmax = i - 1;
	if (minmax == n - 1) {       // degenerate case: all x-coords == xmin
		H[++top] = P[minmin];
		if (P[minmax].y != P[minmin].y) // a  nontrivial segment
			H[++top] = P[minmax];
		H[++top] = P[minmin];            // add polygon endpoint
		return top + 1;
	}

	// Get the indices of points with max x-coord and min|max y-coord
	int maxmin, maxmax = n - 1;
	float xmax = P[n - 1].x;
	for (i = n - 2; i >= 0; i--)
		if (P[i].x != xmax) break;
	maxmin = i + 1;

	// Compute the lower hull on the stack H
	H[++top] = P[minmin];      // push  minmin point onto stack
	i = minmax;
	while (++i <= maxmin)
	{
		// the lower line joins P[minmin]  with P[maxmin]
		if (isLeft(P[minmin], P[maxmin], P[i]) >= 0 && i < maxmin)
			continue;           // ignore P[i] above or on the lower line

		while (top > 0)         // there are at least 2 points on the stack
		{
			// test if  P[i] is left of the line at the stack top
			if (isLeft(H[top - 1], H[top], P[i]) > 0)
				break;         // P[i] is a new hull  vertex
			else
				top--;         // pop top point off  stack
		}
		H[++top] = P[i];        // push P[i] onto stack
	}

	// Next, compute the upper hull on the stack H above  the bottom hull
	if (maxmax != maxmin)      // if  distinct xmax points
		H[++top] = P[maxmax];  // push maxmax point onto stack
	bot = top;                  // the bottom point of the upper hull stack
	i = maxmin;
	while (--i >= minmax)
	{
		// the upper line joins P[maxmax]  with P[minmax]
		if (isLeft(P[maxmax], P[minmax], P[i]) >= 0 && i > minmax)
			continue;           // ignore P[i] below or on the upper line

		while (top > bot)     // at least 2 points on the upper stack
		{
			// test if  P[i] is left of the line at the stack top
			if (isLeft(H[top - 1], H[top], P[i]) > 0)
				break;         // P[i] is a new hull  vertex
			else
				top--;         // pop top point off  stack
		}
		H[++top] = P[i];        // push P[i] onto stack
	}
	if (minmax != minmin)
		H[++top] = P[minmin];  // push  joining endpoint onto stack

	return top + 1;
}




void GeoPolygon::Init(const GrowingArray<GeoVertex>& someVertices)
{
	myVertices = someVertices;	
}

void GeoPolygon::Clear()
{
	myVertices.RemoveAll();
	myPosition = Vec2::ZERO;
}

void GeoPolygon::SetPositionToCenterOfVerices()
{
	int vCount = myVertices.Count();

	Vec2 center = Vec2(0, 0);

	for (int i = 0; i < vCount; ++i)
	{
		center += myVertices[i];
	}

	center.x /= (float)vCount;
	center.y /= (float)vCount;

	myPosition = center;

	Vec2 toPos = myPosition - center;

	for (int i = 0; i < vCount; ++i)
	{
		myVertices[i] -= myPosition;
	}
	
}
void GeoPolygon::MakeVerticesLocalToPosition(const Vec2& aPosition)
{
	myPosition = aPosition;

	int vCount = myVertices.Count();

	Vec2 center = Vec2(0, 0);

	for (int i = 0; i < vCount; ++i)
	{
		center += myVertices[i];
	}

//	center /= vCount;

	Vec2 toPos = myPosition - center;

	for (int i = 0; i < vCount; ++i)
	{
		myVertices[i] += center;
	}

}

void GeoPolygon::GetEdges(GrowingArray<GeoEdge>& someOutEdges) const
{
	if (myVertices.Count() <= 0)
	{
		return;
	}

	someOutEdges.RemoveAll();
	someOutEdges.Init(myVertices.Count());

	for (int i = 0; i < myVertices.Count() - 1; ++i)
	{
		someOutEdges.Add(GeoEdge(myPosition + myVertices[i], myPosition + myVertices[i + 1]));
	}

	//last
	int lastIndex = myVertices.Count() - 1;
	someOutEdges.Add(GeoEdge(myPosition + myVertices[lastIndex], myPosition + myVertices[0]));
}
void GeoPolygon::GetEdgesLocal(GrowingArray<GeoEdge>& someOutEdges) const
{
	if (myVertices.Count() <= 0)
	{
		return;
	}
	someOutEdges.RemoveAll();
	someOutEdges.Init(myVertices.Count());
	for (int i = 0; i < myVertices.Count()-1; ++i)
	{
		someOutEdges.Add(GeoEdge(myVertices[i], myVertices[i + 1]));
	}

	//last
	int lastIndex = myVertices.Count() - 1;
	someOutEdges.Add(GeoEdge(myVertices[lastIndex], myVertices[0]));
}

void GeoPolygon::SetPosition(const Vec2& aPosition)
{
	myPosition = aPosition;
}
Vec2 GeoPolygon::GetPosition()
{
	return myPosition;
}

GrowingArray<GeoVertex>& GeoPolygon::GetVertices()
{
	return myVertices;
}
void GeoPolygon::GetVertices(GrowingArray<GeoVertex>& someOutVertices) const
{
	GetVerticesLocal(someOutVertices);
	int numVerts = someOutVertices.Count();
	for (int i = 0; i < numVerts; ++i)
	{
		someOutVertices[i] += myPosition;
	}
}
void GeoPolygon::GetVerticesLocal(GrowingArray<GeoVertex>& someOutVertices) const
{
	someOutVertices = myVertices;
}
const int GeoPolygon::GetVerticesNum() const
{
	return myVertices.Count();
}

