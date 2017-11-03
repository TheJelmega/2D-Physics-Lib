#include "Physics/Shapes/PolygonShape.h"
#include "Common/BlockAllocator.h"
#include "Physics/Body.h"

namespace P2D {

	PolygonShapeDef::PolygonShapeDef()
		: ShapeDef()
		, numPoints(0)
		, points(nullptr)
	{
	}

	PolygonShape::PolygonShape(const PolygonShapeDef& def, BlockAllocator* pAlloc)
		: Shape(Type::Polygon, def)
		, m_pAlloc(pAlloc)
		, m_NumPoints(def.numPoints)
		, m_Points(nullptr)
	{
		if (m_NumPoints >= 3)
		{
			m_Points = static_cast<f32v2*>(m_pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));
			m_Normals = static_cast<f32v2*>(m_pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));
			memcpy(m_Points, def.points, m_NumPoints * sizeof(f32v2));

			for (u32 i = 0; i < m_NumPoints; ++i)
			{
				u32 i1 = i + 1;
				if (i1 == m_NumPoints)
					i1 = 0;

				f32v2 v0 = m_Points[i];
				f32v2 v1 = m_Points[i1];
				m_Normals[i] = (v1 - v0).Cross(-1.f).Normalize();
			}
			m_MassData.area = CalculateArea();
			PolygonShape::UpdateMass();
		}

		m_Radius = g_SkinThickness;
	}

	PolygonShape::~PolygonShape()
	{
	}

	void PolygonShape::SetAsBox(f32 width, f32 height)
	{
		if (m_Points)
			m_pAlloc->Deallocate(m_Points, m_NumPoints * sizeof(f32v2));

		m_NumPoints = 4;
		m_Points = static_cast<f32v2*>(m_pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));
		m_Normals = static_cast<f32v2*>(m_pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));
		f32 halfWidth = width / 2;
		f32 halfHeight = height / 2;

		m_Points[0] = f32v2(-halfWidth, halfHeight);
		m_Points[1] = f32v2(-halfWidth, -halfHeight);
		m_Points[2] = f32v2(halfWidth, -halfHeight);
		m_Points[3] = f32v2(halfWidth, halfHeight);
		m_Normals[0] = f32v2(-1.f, 0.f);
		m_Normals[1] = f32v2(0.f, -1.f);
		m_Normals[2] = f32v2(1.f, 0.f);
		m_Normals[3] = f32v2(0.f, 1.f);

		m_MassData.area = width * height;
		UpdateMass();
	}

	void PolygonShape::SetAsRegularPolygon(u32 numSides, f32 radius)
	{
		if (numSides < 3)
			return;

		if (m_Points)
			m_pAlloc->Deallocate(m_Points, m_NumPoints * sizeof(f32v2));

		m_NumPoints = numSides;
		m_Points = static_cast<f32v2*>(m_pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));
		m_Normals = static_cast<f32v2*>(m_pAlloc->Allocate(m_NumPoints * sizeof(f32v2)));

		f32 angleStep = Math::Pi<f32> * 2.f / f32(numSides);

		for (u32 i = 0; i < m_NumPoints; ++i)
		{
			m_Points[i] = (f32v2::AxisY * radius).Rotate(f32(i) * angleStep);
		}
		for (u32 i = 0; i < m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == m_NumPoints)
				i1 = 0;

			f32v2 v0 = m_Points[i];
			f32v2 v1 = m_Points[i1];
			m_Normals[i] = (v1 - v0).Cross(-1.f).Normalize();
		}
		m_MassData.area = CalculateArea();
		UpdateMass();
	}

	void PolygonShape::UpdateMass()
	{
		m_MassData.mass = m_Material.density * m_MassData.area;
		m_MassData.centerOfMass = CalculateCenterOfMass();
		m_MassData.shapeInertia = m_MassData.mass + CalculateInertiaOVerMass();
		if (m_pBody)
			UpdateInertia();
	}

	void PolygonShape::SetMass(f32 mass)
	{
		m_MassData.mass = mass;
		m_Material.density = mass / m_MassData.area;
		m_MassData.shapeInertia = m_MassData.mass + CalculateInertiaOVerMass();
		if (m_pBody)
			UpdateInertia();
	}

	void PolygonShape::UpdateInertia()
	{
		m_MassData.inertia = m_MassData.shapeInertia + m_MassData.mass * (m_RelPos + m_MassData.centerOfMass - m_pBody->m_MassData.centerOfMass).SqLength();
	}

	void PolygonShape::UpdateAABB()
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

	void PolygonShape::SetRelPosition(const f32v2& relPos)
	{
		m_RelPos = relPos;
		m_MassData.centerOfMass = CalculateCenterOfMass();
		if (m_pBody)
		{
			UpdateInertia();
			m_pBody->UpdateAABB();
		}
	}

	bool PolygonShape::CheckWinding()
	{
		for (u32 i = 0; i < m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == m_NumPoints)
				i1 = 0;

			f32v2 n0 = m_Normals[0];
			f32v2 n1 = m_Normals[1];
			f32 cross = n0.Cross(n1);
			if (cross < 0.f)
				return false;
		}
		return true;
	}

	f32 PolygonShape::CalculateArea()
	{
		f32 area = 0.f;
		for (u32 i = 0; i < m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == m_NumPoints)
				i1 = 0;
			f32v2 v0 = m_Points[i];
			f32v2 v1 = m_Points[i1];
			area += v0.Cross(v1);
		}
		return area * .5f;
	}

	f32 PolygonShape::CalculateInertiaOVerMass()
	{
		f32 inertia = 0.f;

		for (u32 i = 0; i < m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == m_NumPoints)
				i1 = 0;
			f32v2 v0 = m_Points[i] - m_MassData.centerOfMass;
			f32v2 v1 = m_Points[i1] - m_MassData.centerOfMass;
			f32 x = v0.x * v0.x + v0.x * v1.x + v1.x * v1.x;
			f32 y = v0.y * v0.y + v0.y * v1.y + v1.y * v1.y;
			inertia += (x + y) * v0.Cross(v1);
		}

		return inertia / 12.f;
	}

	f32v2 PolygonShape::CalculateCenterOfMass()
	{
		f32v2 centroid;

		for (u32 i = 0; i < m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == m_NumPoints)
				i1 = 0;
			f32v2 v0 = m_Points[i];
			f32v2 v1 = m_Points[i1];
			centroid += (v0 + v1) * v0.Cross(v1);
		}

		return centroid * (1.f / (6.f * m_MassData.area));
	}
}
