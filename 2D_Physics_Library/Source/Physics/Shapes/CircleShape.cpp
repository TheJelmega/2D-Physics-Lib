#include "Physics/Shapes/CircleShape.h"

namespace P2D {

	CircleShape::CircleShape(const CircleShapeDef& def)
		: Shape(Type::Circle, def.material)
		, m_Radius(def.radius)
	{
		m_Mass = Math::Pi<f32> * m_Radius * m_Radius * m_Material.density;

	}

	CircleShape::~CircleShape()
	{
	}

	void CircleShape::UpdateMass()
	{
		m_Mass = m_Material.density * m_Radius * m_Radius * Math::Pi<f32>;
	}

	void CircleShape::SetMass(f32 mass)
	{
		m_Mass = mass;
		m_Material.density = mass / (m_Radius * m_Radius * Math::Pi<f32>);
	}
}
