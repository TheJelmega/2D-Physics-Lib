#include "Physics/Shapes/EdgeShape.h"
#include "Physics/Body.h"

namespace P2D {


	EdgeShape::EdgeShape(const EdgeShapeDef& def)
		: Shape(Type::Edge, def)
		, m_V0(def.v0)
		, m_V1(def.v1)
	{
		m_Normal = (m_V1 - m_V0).Cross(1.f);
		m_Normal.Normalize();
		m_Radius = g_SkinThickness;
	}

	EdgeShape::~EdgeShape()
	{
	}

	void EdgeShape::UpdateAABB()
	{
		f32v2 v0 = m_RelPos + m_V0;
		v0 = m_pBody->m_Transform.Move(v0);
		f32v2 v1 = m_RelPos + m_V1;
		v1 = m_pBody->m_Transform.Move(v1);

		if (v0.x > v1.x)
		{
			m_AABB.max.x = v0.x;
			m_AABB.min.x = v1.x;
		}
		else
		{
			m_AABB.max.x = v1.x;
			m_AABB.min.x = v0.x;
		}

		if (v0.y > v1.y)
		{
			m_AABB.max.y = v0.y;
			m_AABB.min.y = v1.y;
		}
		else
		{
			m_AABB.max.y = v1.y;
			m_AABB.min.y = v0.y;
		}
		m_AABB.Pad(g_AABBExtension);
	}

	void EdgeShape::SetRelPosition(const f32v2& relPos)
	{
		m_RelPos = relPos;
		m_pBody->UpdateAABB();
	}
}
