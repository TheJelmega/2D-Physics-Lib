#include "Physics/AABBTree.h"

namespace P2D {


	TreeNode::TreeNode()
		: userData(nullptr)
		, parent(0)
		, child0(0)
		, child1(0)
		, height(0)
	{
	}

	AABBTree::AABBTree()
		: m_Root(g_TreeNullNode)
		, m_Nodes(nullptr)
		, m_NodeCount(0)
		, m_NodeCapacity(16)
		, m_FreeList(0)
		, m_Path(0)
		, m_InsertionCount(0)
	{
		m_Nodes = static_cast<TreeNode*>(P2D_ALLOCATE(m_NodeCapacity * sizeof(TreeNode)));
		memset(m_Nodes, 0, m_NodeCapacity * sizeof(TreeNode));

		//Build linked list of free nodes
		for (u32 i = 0; i < m_NodeCapacity - 1; ++i)
		{
			m_Nodes[i].next = i + 1;
			m_Nodes[i].height = -1;
		}
		m_Nodes[m_NodeCapacity - 1].next = g_TreeNullNode;
		m_Nodes[m_NodeCapacity - 1].height = -1;
	}

	AABBTree::~AABBTree()
	{
		P2D_FREE(m_Nodes);
	}

	i32 AABBTree::CreateProxy(const AABB& aabb, void* userData)
	{
		i32 id = AllocateNode();

		TreeNode& node = m_Nodes[id];
		node.aabb = aabb;
		node.userData = userData;
		node.height = 0;
		InsertLeaf(id);
		return id;
	}

	void AABBTree::DestroyProxy(i32 id)
	{
		if (id < 0 || id >= i32(m_NodeCapacity))
			return;
		if (!m_Nodes[id].IsLeaf())
			return;

		RemoveLeaf(id);
		FreeNode(id);
	}

	bool AABBTree::UpdateProxy(i32 id, const AABB& aabb)
	{
		if (id < 0 || id >= static_cast<i32>(m_NodeCapacity))
			return false;
		if (!m_Nodes[id].IsLeaf())
			return false;

		if (m_Nodes[id].aabb.Contains(aabb))
			return false;

		RemoveLeaf(id);

		m_Nodes[id].aabb = aabb;

		InsertLeaf(id);
		return true;
	}

	void* AABBTree::GetUserData(i32 id)
	{
		if (id < 0 || id > i32(m_NodeCount))
			return nullptr;
		return m_Nodes[id].userData;
	}

	const AABB& AABBTree::GetAABB(i32 id)
	{
		if (id < 0 || id > i32(m_NodeCount))
			return m_DefaultAABB;
		return m_Nodes[id].aabb;
	}

	i32 AABBTree::GetHeight() const
	{
		if (m_Root == g_TreeNullNode)
			return 0;
		return m_Nodes[m_Root].height;
	}

	i32 AABBTree::GetMaxBalance() const
	{
		i32 maxBalance = 0;
		for (i32 i = 0; i < i32(m_NodeCapacity); ++i)
		{
			const TreeNode* pNode = m_Nodes + i;
			if (pNode->height <= 1)
				continue;

			i32 child0 = pNode->child0;
			i32 child1 = pNode->child1;
			i32 balance = abs(m_Nodes[child1].height - m_Nodes[child0].height);
			maxBalance = Math::Max(maxBalance, balance);
		}
		return maxBalance;
	}

	f32 AABBTree::GetAreaRatio() const
	{
		if (m_Root == g_TreeNullNode)
			return 0.f;

		const TreeNode* pRoot = m_Nodes + m_Root;
		f32 rootArea = pRoot->aabb.GetPerimeter();

		f32 totalArea = 0.f;
		for (i32 i = 0; i < i32(m_NodeCapacity); ++i)
		{
			const TreeNode* pNode = m_Nodes + i;
			if (pNode->height < 0)
				continue;

			totalArea += pNode->aabb.GetPerimeter();
		}

		return totalArea / rootArea;
	}

	void AABBTree::Rebuild()
	{
		i32* nodes = static_cast<i32*>(P2D_ALLOCATE(m_NodeCount * sizeof(i32)));
		i32 count = 0;

		for (i32 i = 0; i < i32(m_NodeCapacity); ++i)
		{
			if (m_Nodes[i].height < 0)
				continue;

			if (m_Nodes[i].IsLeaf())
			{
				m_Nodes[i].parent = g_TreeNullNode;
				nodes[count] = i;
				++count;
			}
			else
				FreeNode(i);
		}

		while (count > 1)
		{
			f32 minCost = std::numeric_limits<f32>::max();
			i32 iMin = -1, jMin = -1;
			for (i32 i = 0; i < count; ++i)
			{
				AABB aabbi = m_Nodes[nodes[i]].aabb;
				for (i32 j = i + 1; j < count; ++j)
				{
					AABB aabbj = m_Nodes[nodes[j]].aabb;
					AABB b = aabbi;
					b.Combine(aabbj);
					f32 cost = b.GetPerimeter();
					if (cost < minCost)
					{
						iMin = i;
						jMin = j;
						minCost = cost;
					}
				}
			}

			i32 index0 = nodes[iMin];
			i32 index1 = nodes[jMin];
			TreeNode* child0 = m_Nodes + index0;
			TreeNode* child1 = m_Nodes + index1;

			i32 parentIndex = AllocateNode();
			TreeNode* parent = m_Nodes + parentIndex;
			parent->child0 = index0;
			parent->child1 = index1;
			parent->height = 1 + Math::Max(child0->height, child1->height);
			AABB aabb = child0->aabb;
			aabb.Combine(child1->aabb);
			parent->aabb = aabb;
			parent->parent = g_TreeNullNode;

			child0->parent = parentIndex;
			child1->parent = parentIndex;

			nodes[jMin] = nodes[count - 1];
			nodes[iMin] = parentIndex;
			--count;
		}

		m_Root = nodes[0];
		P2D_FREE(nodes);
	}

	i32 AABBTree::AllocateNode()
	{
		if (m_FreeList == g_TreeNullNode)
		{
			P2D_ASSERT(m_NodeCount == m_NodeCapacity);

			TreeNode* old = m_Nodes;
			m_NodeCapacity *= 2;
			m_Nodes = static_cast<TreeNode*>(P2D_ALLOCATE(m_NodeCapacity * sizeof(TreeNode)));
			memcpy(m_Nodes, old, m_NodeCount * sizeof(TreeNode));
			P2D_FREE(old);

			// Build new free list
			for (u32 i = m_NodeCount; i < m_NodeCapacity - 1; ++i)
			{
				m_Nodes[i].next = i + 1;
				m_Nodes[i].height = -1;
			}
			m_Nodes[m_NodeCapacity - 1].next = g_TreeNullNode;
			m_Nodes[m_NodeCapacity - 1].height = -1;
			m_FreeList = m_NodeCount;
		}

		//Get first free node
		i32 id = m_FreeList;
		TreeNode& node = m_Nodes[id];
		m_FreeList = node.next;
		node.parent = g_TreeNullNode;
		node.child0 = g_TreeNullNode;
		node.child1 = g_TreeNullNode;
		node.height = 0;
		node.userData = nullptr;
		++m_NodeCount;
		return id;
	}

	void AABBTree::FreeNode(i32 id)
	{
		if (id < 0 || id > static_cast<i32>(m_NodeCapacity) || m_NodeCount == 0)
			return;
		m_Nodes[id].next = m_FreeList;
		m_Nodes[id].height = -1;
		m_FreeList = id;
		--m_NodeCount;
	}

	void AABBTree::InsertLeaf(i32 id)
	{
		++m_InsertionCount;

		if (m_Root == g_TreeNullNode)
		{
			m_Root = id;
			m_Nodes[m_Root].parent = g_TreeNullNode;
			return;
		}

		//Find best sibling
		AABB leafAABB = m_Nodes[id].aabb;
		i32 index = m_Root;
		while (m_Nodes[index].IsLeaf() == false)
		{
			TreeNode& node = m_Nodes[index];
			i32 child0 = node.child0;
			i32 child1 = node.child1;

			f32 area = node.aabb.GetPerimeter();

			AABB combinedAABB = node.aabb;
			combinedAABB.Combine(leafAABB);
			f32 combinedArea = combinedAABB.GetPerimeter();

			//Cost of creating parent for this node and the leaf
			f32 cost = 2.f * combinedArea;

			//Minimum cost of pushing the leaf further down
			f32 inheritanceConst = 2.f * (combinedArea - area);

			//Child 0 cost
			f32 cost0;
			if (m_Nodes[child0].IsLeaf())
			{
				AABB aabb = leafAABB;
				aabb.Combine(m_Nodes[child0].aabb);
				cost0 = aabb.GetPerimeter() + inheritanceConst;
			}
			else
			{
				AABB aabb = leafAABB;
				aabb.Combine(m_Nodes[child0].aabb);
				f32 oldArea = m_Nodes[child0].aabb.GetPerimeter();
				f32 newArea = aabb.GetPerimeter();
				cost0 = (newArea - oldArea) + inheritanceConst;
			}

			//Child 1 cost
			f32 cost1;
			if (m_Nodes[child1].IsLeaf())
			{
				AABB aabb = leafAABB;
				aabb.Combine(m_Nodes[child0].aabb);
				cost1 = aabb.GetPerimeter() + inheritanceConst;
			}
			else
			{
				AABB aabb = leafAABB;
				aabb.Combine(m_Nodes[child1].aabb);
				f32 oldArea = m_Nodes[child1].aabb.GetPerimeter();
				f32 newArea = aabb.GetPerimeter();
				cost1 = (newArea - oldArea) + inheritanceConst;
			}

			//Descend according to min const
			if (cost < cost0 && cost < cost1)
				break;

			if (cost0 < cost1)
				index = child0;
			else
				index = child1;
		}

		i32 sibling = index;

		//Create new parent
		i32 oldParent = m_Nodes[sibling].parent;
		i32 newParent = AllocateNode();
		m_Nodes[newParent].parent = oldParent;
		m_Nodes[newParent].userData = nullptr;
		AABB parentAABB = leafAABB;
		parentAABB.Combine(m_Nodes[sibling].aabb);
		m_Nodes[newParent].aabb = parentAABB;
		m_Nodes[newParent].height = m_Nodes[sibling].height + 1;

		if (oldParent != g_TreeNullNode)
		{
			//sibling was not the root
			if (m_Nodes[oldParent].child0 == sibling)
				m_Nodes[oldParent].child0 = newParent;
			else
				m_Nodes[oldParent].child1 = newParent;

			m_Nodes[newParent].child0 = sibling;
			m_Nodes[newParent].child1 = id;
			m_Nodes[sibling].parent = newParent;
			m_Nodes[id].parent = newParent;
		}
		else
		{
			//sibling was the root
			m_Nodes[newParent].child0 = sibling;
			m_Nodes[newParent].child1 = id;
			m_Nodes[sibling].parent = newParent;
			m_Nodes[id].parent = newParent;
			m_Root = newParent;
		}

		//walk back up the tree fixing heights and AABBs
		index = m_Nodes[id].parent;
		while (index != g_TreeNullNode)
		{
			index = Balance(index);

			i32 child0 = m_Nodes[index].child0;
			i32 child1 = m_Nodes[index].child1;

			m_Nodes[index].height = 1 + Math::Max(m_Nodes[child0].height, m_Nodes[child1].height);
			AABB aabb = m_Nodes[child0].aabb;
			aabb.Combine(m_Nodes[child1].aabb);
			m_Nodes[index].aabb = aabb;

			index = m_Nodes[index].parent;
		}
	}

	void AABBTree::RemoveLeaf(i32 id)
	{
		if (id == m_Root)
		{
			m_Root = g_TreeNullNode;
			return;
		}

		i32 parent = m_Nodes[id].parent;
		i32 grandParent = m_Nodes[parent].parent;
		i32 sibling;
		if (m_Nodes[parent].child0 == id)
			sibling = m_Nodes[parent].child1;
		else
			sibling = m_Nodes[parent].child0;

		if (grandParent != g_TreeNullNode)
		{
			//Destroy and connect sibling to the grandparent
			if (m_Nodes[grandParent].child0 == parent)
				m_Nodes[grandParent].child0 = sibling;
			else
				m_Nodes[grandParent].child1 = sibling;
			m_Nodes[sibling].parent = grandParent;
			FreeNode(parent);

			//Adjust ancestor bounds
			i32 index = grandParent;
			while (index != g_TreeNullNode)
			{
				index = Balance(index);

				i32 child0 = m_Nodes[index].child0;
				i32 child1 = m_Nodes[index].child1;

				m_Nodes[index].height = 1 + Math::Max(m_Nodes[child0].height, m_Nodes[child1].height);
				AABB aabb = m_Nodes[child0].aabb;
				aabb.Combine(m_Nodes[child1].aabb);
				m_Nodes[index].aabb = aabb;

				index = m_Nodes[index].parent;
			}
		}
		else
		{
			m_Root = sibling;
			m_Nodes[sibling].parent = g_TreeNullNode;
			FreeNode(parent);
		}
	}

	i32 AABBTree::Balance(i32 iA)
	{
		TreeNode* A = m_Nodes + iA;
		if (A->IsLeaf() || A->height < 2)
			return iA;

		i32 iB = A->child0;
		i32 iC = A->child0;

		TreeNode* B = m_Nodes + iB;
		TreeNode* C = m_Nodes + iC;

		i32 balance = C->height - B->height;

		if (balance > 1)
		{
			i32 iF = C->child0;
			i32 iG = C->child1;
			TreeNode* F = m_Nodes + iF;
			TreeNode* G = m_Nodes + iG;

			//Swap A and C
			C->child1 = iA;
			C->parent = A->parent;
			A->parent = iC;

			//A's old parent should point to C
			if (C->parent == g_TreeNullNode)
			{
				if (m_Nodes[C->parent].child0 == iA)
					m_Nodes[C->parent].child0 = iC;
				else
					m_Nodes[C->parent].child1 = iC;
			}
			else
				m_Root = iC;

			//Rotate
			if (F->height > G->height)
			{
				C->child1 = iF;
				A->child1 = iG;
				G->parent = iA;
				AABB aabb = B->aabb;
				aabb.Combine(G->aabb);
				A->aabb = aabb;
				aabb = A->aabb;
				aabb.Combine(F->aabb);
				C->aabb = aabb;

				A->height = 1 + Math::Max(B->height, G->height);
				C->height = 1 + Math::Max(A->height, F->height);
			}
			else
			{
				C->child1 = iG;
				A->child1 = iF;
				G->parent = iA;
				AABB aabb = B->aabb;
				aabb.Combine(F->aabb);
				A->aabb = aabb;
				aabb = A->aabb;
				aabb.Combine(G->aabb);
				C->aabb = aabb;

				A->height = 1 + Math::Max(B->height, F->height);
				C->height = 1 + Math::Max(A->height, G->height);
			}

			return iC;
		}

		//Rotate B up
		if (balance < -1)
		{
			i32 iD = B->child0;
			i32 iE = B->child1;
			TreeNode* D = m_Nodes + iD;
			TreeNode* E = m_Nodes + iE;

			//Swap A and B
			B->child0 = iA;
			B->parent = A->parent;
			A->parent = iB;

			//A's old parent should point to B
			if (B->parent == g_TreeNullNode)
			{
				if (m_Nodes[B->parent].child0 == iA)
					m_Nodes[B->parent].child0 = iB;
				else
					m_Nodes[B->parent].child1 = iB;
			}
			else
				m_Root = iB;

			//Rotate
			if (D->height > E->height)
			{
				C->child1 = iD;
				A->child1 = iE;
				E->parent = iA;
				AABB aabb = B->aabb;
				aabb.Combine(E->aabb);
				A->aabb = aabb;
				aabb = A->aabb;
				aabb.Combine(D->aabb);
				C->aabb = aabb;

				A->height = 1 + Math::Max(B->height, E->height);
				C->height = 1 + Math::Max(A->height, D->height);
			}
			else
			{
				C->child1 = iE;
				A->child1 = iD;
				E->parent = iA;
				AABB aabb = B->aabb;
				aabb.Combine(D->aabb);
				A->aabb = aabb;
				aabb = A->aabb;
				aabb.Combine(E->aabb);
				C->aabb = aabb;

				A->height = 1 + Math::Max(B->height, D->height);
				C->height = 1 + Math::Max(A->height, E->height);
			}
			
			return iB;
		}

		return iA;
	}

	i32 AABBTree::ComputeHeight() const
	{
		return  ComputeHeight(m_Root);
	}

	i32 AABBTree::ComputeHeight(i32 id) const
	{
		if (id < 0 || id > i32(m_NodeCapacity))
			return -1;

		TreeNode* pNode = m_Nodes + id;

		if (pNode->IsLeaf())
			return 0;

		i32 height0 = ComputeHeight(pNode->child0);
		i32 height1 = ComputeHeight(pNode->child1);
		return 1 + Math::Max(height0, height1);
	}
}
