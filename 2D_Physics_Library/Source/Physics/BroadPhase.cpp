#include "Physics/BroadPhase.h"
#include "Physics/World.h"
#include "Physics/Body.h"
#include <iostream>

namespace P2D {


	BroadPhase::BroadPhase(World* pWorld)
		:m_pWorld(pWorld)
	{
	}

	void BroadPhase::UpdatePairs(ContactManager* callback)
	{
		if (!m_pWorld->m_pBodyList)
			return;

		for (Body* pBody0 = m_pWorld->m_pBodyList; pBody0->m_pNext; pBody0 = pBody0->m_pNext)
		{
			for (Body* pBody1 = pBody0->m_pNext; pBody1; pBody1 = pBody1->m_pNext)
			{
				if (!pBody0->m_AABB.Overlaps(pBody1->m_AABB))
					continue;

				for (Shape* pShape0 = pBody0->m_pShape; pShape0; pShape0 = pShape0->m_pNext)
				{
					for (Shape* pShape1 = pBody1->m_pShape; pShape1; pShape1 = pShape1->m_pNext)
					{
						if (pShape0->m_AABB.Overlaps(pShape1->m_AABB))
						{
							// Add pair
							if (pShape0->m_pBody->GetBodyType() != BodyType::Kinematic)
								callback->AddPair(pShape1, pShape0);
							else
								callback->AddPair(pShape0, pShape1);
						}
					}
				}
			}
		}
	}
}
