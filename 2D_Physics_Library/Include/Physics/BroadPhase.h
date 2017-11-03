#pragma once
#include "Common/BaseTypes.h"
#include "AABBTree.h"

namespace P2D {
	class Shape;
	struct AABB;

	class World;
	class ContactManager;

	class BroadPhase
	{
	public:
		BroadPhase(World* pWorld);
		~BroadPhase();

		void AddShape(Shape* pShape);
		void RemoveShape(Shape* pShape);
		void UpdateShape(Shape* pShape);

		/**
		 * Update collsion broadphase
		 * @param[in] callback ContactManager to add pairs to
		 */
		void UpdatePairs(ContactManager* callback);

		/**
		 * @note shapeArr needs to have enough capacity to store shapes
		 * @param[out] pOutShape	First shape
		 */
		void Query(const AABB& aabb, Shape* pOutShape);

	private:
		friend class AABBTree;

		bool QueryCallback(i32 id);

		World* m_pWorld;
		AABBTree m_Tree;

		i32* m_ProxyIds;
		u32 m_ProxyCapacity;
		u32 m_ProxyCount;
	};

}
