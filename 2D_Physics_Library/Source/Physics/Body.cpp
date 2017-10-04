#include "Physics/Body.h"

namespace P2D {

	BodyDef::BodyDef()
		: type(BodyType::Dynamic)
		, angle(0.f)
		, active(true)
		, awake(true)
	{
	}

	Body::Body(const BodyDef& def)
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pChild(nullptr)
		, m_pParent(nullptr)
		, m_pShape(nullptr)
		, m_ShapeCount(0)
		, m_Type(def.type)
		, m_Position(def.position)
		, m_Angle(def.angle)
		, m_AngularVelocity(0.f)
		, m_AngularDamping(0.f)
		, m_Torque(0.f)
		, m_Active(def.active)
		, m_Awake(def.awake)

	{
	}

	Body::~Body()
	{
	}

	void Body::AddShape(Shape* pShape)
	{
		if (m_pShape)
			pShape->m_pNext = m_pShape;
		m_pShape = pShape;
	}

	void Body::ApplyForce(f32v2 force, bool wake)
	{
	}

	void Body::ApplyForce(f32v2 force, f32v2 point, bool wake)
	{
	}

	void Body::ApplyTorque(f32 torque, bool wake)
	{
	}

	void Body::ApplyImpulse(f32v2 impulse, bool wake)
	{
		f32 mass = GetMass();
		m_LinearVelocity += impulse / mass;

		if (wake)
			SetAwake(true);
	}

	void Body::ApplyImpulse(f32v2 impulse, f32v2 point, bool wake)
	{
	}

	void Body::ApplyAngularImpulse(f32 impulse, bool wake)
	{
	}

	void Body::AddChild(Body* pChild)
	{
		if (m_pChild)
		{
			pChild->m_pNext = m_pChild;
			m_pChild->m_pPrev = pChild;
		}
		else
		{
			pChild->m_pNext = nullptr;
		}
		pChild->m_pPrev = nullptr;
		pChild->m_pParent = this;
		m_pChild = pChild;
	}

	f32 Body::GetMass()
	{
		f32 mass = 0.f;
		for (Shape* pShape = m_pShape; pShape; pShape = pShape->m_pNext)
		{
			mass += pShape->m_Mass;
		}
		return mass;
	}

	void Body::SetAwake(bool awake)
	{
		if (m_Awake == awake)
			return;
	}

	void Body::SetActive(bool active)
	{
	}
}
