#pragma once
#include "Math/Math.h"

namespace P2D {
	
	constexpr i32 g_TreeNullNode = -1;

	struct TreeNode
	{
		AABB aabb;
		void* userData;

#pragma warning(push)
#pragma warning(disable: 4201) //Disable error C4201 (nameless struct/union)
		union
		{
			i32 parent;
			i32 next;
		};
#pragma warning(pop)

		i32 child0;
		i32 child1;

		i32 height;

		bool IsLeaf() const { return child0 == g_TreeNullNode; }

		TreeNode();
	};
	
	//AABB tree based on b2DynamicTree
	class AABBTree
	{
	public:
		AABBTree();
		~AABBTree();

		i32 CreateProxy(const AABB& aabb, void* UserData);
		void DestroyProxy(i32 id);
		bool UpdateProxy(i32 id, const AABB& aabb);
		void* GetUserData(i32 id);
		const AABB& GetAABB(i32 id);
		template<typename T>
		void Query(T* callback, const AABB& aabb);

		i32 GetHeight() const;
		i32 GetMaxBalance() const;
		f32 GetAreaRatio() const;
		void Rebuild();

	private:
		i32 AllocateNode();
		void FreeNode(i32 id);

		void InsertLeaf(i32 id);
		void RemoveLeaf(i32 id);

		i32 Balance(i32 id);

		i32 ComputeHeight() const;
		i32 ComputeHeight(i32 id) const;

		i32 m_Root;
		TreeNode* m_Nodes;
		u32 m_NodeCount;
		u32 m_NodeCapacity;
		i32 m_FreeList;

		// Used to incrementally traverse tree for rebalancing
		u32 m_Path;
		
		i32 m_InsertionCount;

		const AABB m_DefaultAABB;
	};

	template <typename T>
	void AABBTree::Query(T* callback, const AABB& aabb)
	{
		u32 stackCapacity = 256;
		i32* stack = static_cast<i32*>(P2D_ALLOCATE(stackCapacity * sizeof(i32)));
		stack[0] = m_Root;
		u32 count = 1;

		while (count > 0)
		{
			i32 id = stack[--count];
			if (id == g_TreeNullNode)
				continue;

			const TreeNode* pNode = m_Nodes + id;

			if (pNode->aabb.Overlaps(aabb))
			{
				if (pNode->IsLeaf())
				{
					bool proceed = callback->QueryCallback(id);
					if (!proceed)
					{
						P2D_FREE(stack);
						return;
					}
				}
				else
				{
					if (count + 2 > stackCapacity)
					{
						i32* old = stack;
						stackCapacity *= 2;
						stack = static_cast<i32*>(P2D_ALLOCATE(stackCapacity * sizeof(i32)));
						memcpy(stack, old, count * sizeof(i32));
						P2D_FREE(old);
					}

					stack[count++] = pNode->child0;
					stack[count++] = pNode->child1;
				}
			}
		}
	}

}
