#include "Physics/Shapes/ChainShape.h"
#include "Physics/Body.h"

namespace P2D {

	ChainShapeDef::ChainShapeDef()
		: ShapeDef()
		, numPoints(0)
		, points(nullptr)
	{
	}

	ChainShape::ChainShape(const ChainShapeDef& def, BlockAllocator* pAlloc)
		: Shape(Type::Chain, def)
		, m_pAlloc(pAlloc)
		, m_NumPoints(def.numPoints)
		, m_Points(nullptr)
	{
		if (m_NumPoints > 1)
		{
			m_Points = static_cast<f32v2*>(pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));
			memcpy(m_Points, def.points, m_NumPoints * sizeof(f32v2));
		}
	}

	ChainShape::~ChainShape()
	{
		m_pAlloc->Deallocate(m_Points, m_NumPoints * sizeof(f32v2));
	}

	void ChainShape::UpdateAABB()
	{
		if (m_NumPoints == 0)
			return;

		f32v2 point = m_Points[0] + m_RelPos;
		Transform transform = m_pBody->m_Transform;
		point = transform.Move(point);

		m_AABB.min = m_AABB.max = point;
		for (u32 i = 1; i < m_NumPoints; ++i)
		{
			point = m_Points[i] + m_RelPos;
			point = transform.Move(point);
			m_AABB.max.x = Math::Max(m_AABB.max.x, point.x);
			m_AABB.max.y = Math::Max(m_AABB.max.y, point.y);
			m_AABB.min.x = Math::Min(m_AABB.min.x, point.x);
			m_AABB.min.y = Math::Min(m_AABB.min.y, point.y);
		}

		m_AABB.Pad(g_AABBExtension);
	}

	void ChainShape::SetRelPosition(const f32v2& relPos)
	{
		m_RelPos = relPos;
		m_pBody->UpdateAABB();
	}

	void ChainShape::GetChildEdge(EdgeShape* pEdge, u32 childIndex)
	{
		if (childIndex >= m_NumPoints)
			return;

		pEdge->m_RelPos = m_RelPos;
		pEdge->m_V0 = m_Points[childIndex];
		pEdge->m_V1 = m_Points[childIndex + 1];
		pEdge->m_Normal = (pEdge->m_V1 - pEdge->m_V0).Cross(1.f);
		pEdge->m_Normal.Normalize();
		pEdge->UpdateAABB();
	}
}
