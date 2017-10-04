#include "Physics/Shapes/Shape.h"

namespace P2D {


	Shape::Shape()
		: m_pNext(nullptr)
		, m_Type(Type::None)
		, m_Mass(0)
	{
	}

	Shape::Shape(Type type, const Material& mat)
		: m_pNext(nullptr)
		, m_Type(type)
		, m_Material(mat)
		, m_Mass(0)
	{
	}

	Shape::~Shape()
	{
	}

	void Shape::UpdateMass()
	{
	}

	void Shape::SetMass(f32 mass)
	{
	}
}
