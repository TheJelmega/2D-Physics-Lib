#include "Physics/BroadPhase.h"
#include "Physics/World.h"
#include "Physics/Body.h"
#include <iostream>

namespace P2D {


	BroadPhase::BroadPhase(World* pWorld)
		: m_pWorld(pWorld)
		, m_ProxyIds(nullptr)
		, m_ProxyCapacity(256)
		, m_ProxyCount(0)
	{
		m_ProxyIds = static_cast<i32*>(P2D_ALLOCATE(m_ProxyCapacity * sizeof(i32)));
	}

	BroadPhase::~BroadPhase()
	{
		P2D_FREE(m_ProxyIds);
	}

	void BroadPhase::AddShape(Shape* pShape)
	{
		pShape->m_ProxyId = m_Tree.CreateProxy(pShape->m_AABB, pShape);
	}

	void BroadPhase::RemoveShape(Shape* pShape)
	{
		m_Tree.DestroyProxy(pShape->m_ProxyId);
		pShape->m_ProxyId = -1;
	}

	void BroadPhase::UpdateShape(Shape* pShape)
	{
		m_Tree.UpdateProxy(pShape->m_ProxyId, pShape->m_AABB);
	}

	void BroadPhase::UpdatePairs(ContactManager* callback)
	{
		if (!m_pWorld->m_pBodyList)
			return;

		//faster in debug, slower in release
		/*for (Body* pBody0 = m_pWorld->m_pBodyList; pBody0->m_pNext; pBody0 = pBody0->m_pNext)
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
		}*/

		m_ProxyCount = 0;

		for (Body* pBody0 = m_pWorld->m_pBodyList; pBody0->m_pNext; pBody0 = pBody0->m_pNext)
		{
			if (pBody0->m_Type == BodyType::Static || !pBody0->m_Active || !pBody0->m_Awake)
				continue;

			for (Shape* pShape0 = pBody0->m_pShape; pShape0; pShape0 = pShape0->m_pNext)
			{
				m_Tree.Query(this, pShape0->GetAABB());
				for (u32 i = 0; i < m_ProxyCount; ++i)
				{
					callback->AddPair(pShape0, static_cast<Shape*>(m_Tree.GetUserData(m_ProxyIds[i])));
				}
				m_ProxyCount = 0;
			}
		}
	}

	void BroadPhase::Query(const AABB& aabb, Shape* pOutShape)
	{
		pOutShape = nullptr;

		/*for (Body* pBody = m_pWorld->m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			if (!pBody->m_AABB.Overlaps(aabb))
				continue;

			for (Shape* pShape = pBody->m_pShape; pShape; pShape->m_pNext)
			{
				if (!pShape->m_AABB.Overlaps(aabb))
					continue;

				pShape->m_pNextQueried = pOutShape;
				pOutShape = pShape;
			}
		}*/
		m_ProxyCount = 0;
		m_Tree.Query(this, aabb);
		Shape* pShape = nullptr;
		for (u32 i = 0; i < m_ProxyCount; ++i)
		{
			pShape = static_cast<Shape*>(m_Tree.GetUserData(i));
			pShape->m_pNextQueried = pOutShape;
			pOutShape = pShape;
		}
		m_ProxyCount = 0;
	}

	bool BroadPhase::QueryCallback(i32 id)
	{
		if (m_ProxyCount == m_ProxyCapacity)
		{
			i32* old = m_ProxyIds;
			m_ProxyCapacity *= 2;
			m_ProxyIds = static_cast<i32*>(P2D_ALLOCATE(m_ProxyCapacity * sizeof(i32)));
			memcpy(m_ProxyIds, old, m_ProxyCount * sizeof(i32));
			P2D_FREE(old);
		}

		m_ProxyIds[m_ProxyCount++] = id;

		return true;
	}
}
