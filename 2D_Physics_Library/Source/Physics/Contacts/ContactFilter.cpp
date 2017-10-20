#include "Physics/Contacts/ContactFilter.h"
#include "Physics/Shapes/Shape.h"

namespace P2D {


	ContactFilter::~ContactFilter()
	{
	}

	bool ContactFilter::ShouldCollide(Shape* pShape0, Shape* pShape1)
	{
		const CollisionFilter& filter0 = pShape0->GetFilterData();
		const CollisionFilter& filter1 = pShape1->GetFilterData();

		if (filter0.group == filter1.group && filter0.group != 0)
		{
			return filter0.group > 0;
		}

		return (filter0.category & filter1.collisionMask) && (filter1.category && filter0.collisionMask);
	}
}
