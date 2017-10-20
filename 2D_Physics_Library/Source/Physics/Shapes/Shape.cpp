#include "Physics/Shapes/Shape.h"

namespace P2D {


	MassData::MassData()
		: mass(0)
		, inertia(0)
		, area(0)
	{
	}

	CollisionFilter::CollisionFilter()
		: category(0x0001)
	    , collisionMask(0xFFFF)
	    , group(0)
	{
	}

	ShapeDef::ShapeDef()
		: isSensor(false)
	{
	}

	Shape::Shape()
		: m_pNext(nullptr)
		, m_pBody(nullptr)
		, m_Type(Type::None)
		, m_Sensor(false)
		, m_Radius(0)
	{
	}

	Shape::Shape(Type type, const ShapeDef& def)
		: m_pNext(nullptr)
		, m_pBody(nullptr)
		, m_Type(type)
		, m_Material(def.material)
		, m_RelPos(def.relpos)
		, m_Filter(def.collisionFilter)
		, m_Sensor(def.isSensor)
		, m_Radius(0)
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
		m_MassData.mass = mass;
	}

	void Shape::UpdateAABB()
	{
	}
}
