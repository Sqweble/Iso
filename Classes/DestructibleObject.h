#ifndef _DESTRUCTIBLE_OBJECT_H_
#define _DESTRUCTIBLE_OBJECT_H_

#include "GameObject.h"

#include "Polygon.h"

#include "GrowingArray.h"

class DestructibleObject : public GameObject
{
public:
	bool init();
	static DestructibleObject* create();
	
	void Init(const GrowingArray<GeoVertex>& aPolygon);

	void update(float aDeltaTime);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	void Cut(GeoPolygon* aCuttingPolygon);

	//When PhysicsBody is active transform breaks...
	const Mat4& getNodeToParentTransform() const;
	

private:
	GeoPolygon* myPolygon;
	PhysicsBody* myPhysicsBody;
	
	//GrowingArray<GeoPolygon*> myPolygons;
	GrowingArray<GrowingArray<GeoVertex>> myNewPolygons;



};




#endif