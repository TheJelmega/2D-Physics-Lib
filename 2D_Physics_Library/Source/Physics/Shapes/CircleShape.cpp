#include "Physics/Shapes/CircleShape.h"
#include "Physics/Body.h"

namespace P2D {

	CircleShapeDef::CircleShapeDef()
		: ShapeDef()
		, radius(0)
	{
	}

	CircleShape::CircleShape(const CircleShapeDef& def)
		: Shape(Type::Circle, def)
	{
		m_Radius = def.radius;
		m_MassData.area = m_Radius * m_Radius * Math::Pi<f32>;
		//m_MassData.centerOfMass = f32v2::Zero;
		CircleShape::UpdateMass();
	}

	CircleShape::~CircleShape()
	{
	}

	void CircleShape::UpdateMass()
	{
		m_MassData.mass = m_Material.density * m_MassData.area;
		m_MassData.inertia = m_MassData.mass * (.5f * m_Radius * m_Radius + m_MassData.centerOfMass.SqLength());
	}

	void CircleShape::SetMass(f32 mass)
	{
		m_MassData.mass = mass;
		m_Material.density = mass / m_MassData.area;
		m_MassData.inertia = mass * (0.5f * m_Radius * m_Radius + m_MassData.centerOfMass.SqLength());
	}

	void CircleShape::SetRelPosition(const f32v2& relPos)
	{
		m_RelPos = relPos;
		m_MassData.centerOfMass = relPos;
		m_pBody->UpdateAABB();
	}

	void CircleShape::UpdateAABB()
	{
		f32v2 pos = m_pBody->m_Transform.Move(m_RelPos);
		m_AABB.min = pos - m_Radius;
		m_AABB.max = pos + m_Radius;
		m_AABB.Pad(g_AABBExtension);
	}
}
