#include "Physics/Body.h"

namespace P2D {

	BodyDef::BodyDef()
		: type(BodyType::Dynamic)
		, angle(0.f)
		, active(true)
		, awake(true)
	{
	}

	Body::Body(const BodyDef& def, World* pWorld)
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pWorld(pWorld)
		, m_pShape(nullptr)
		, m_ShapeCount(0)
		, m_pContactList(nullptr)
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
		pShape->m_pBody = this;
		pShape->UpdateAABB();
		m_AABB.Combine(pShape->m_AABB);
		++m_ShapeCount;

		//Update mass data
		f32v2 tempCenterOfMass = m_MassData.mass * m_MassData.centerOfMass + pShape->m_MassData.mass * pShape->m_MassData.centerOfMass;
		m_MassData.mass += pShape->m_MassData.mass;
		m_MassData.centerOfMass = tempCenterOfMass / m_MassData.mass;
		m_MassData.inertia += pShape->m_MassData.inertia;
	}

	void Body::ApplyForce(f32v2 force, bool wake)
	{
		if (m_Type != BodyType::Dynamic)
			return;

		if (wake)
			SetAwake(true);

		//Only apply force when awake
		if (m_Awake)
			m_Force += force;
	}

	void Body::ApplyForce(f32v2 force, f32v2 point, bool wake)
	{
		if (m_Type != BodyType::Dynamic)
			return;

		if (wake)
			SetAwake(true);

		//Only apply force when awake
		if (m_Awake)
		{
			m_Force += force;
			m_Torque += (point - m_Position).Cross(force);
		}
	}

	void Body::ApplyTorque(f32 torque, bool wake)
	{
		if (m_Type != BodyType::Dynamic)
			return;

		if (wake)
			SetAwake(true);

		//Only apply torque when awake
		if (m_Awake)
			m_Torque += torque;
	}

	void Body::ApplyImpulse(f32v2 impulse, bool wake)
	{
		if (m_Type != BodyType::Dynamic)
			return;

		if (wake)
			SetAwake(true);

		//Only apply impulse when awake
		if (m_Awake)
		{
			m_LinearVelocity += impulse / m_MassData.mass;
		}
	}

	void Body::ApplyImpulse(f32v2 impulse, f32v2 point, bool wake)
	{
		if (m_Type != BodyType::Dynamic)
			return;

		if (wake)
			SetAwake(true);

		//Only apply impulse when awake
		if (m_Awake)
		{
			m_LinearVelocity += impulse / m_MassData.mass;
			f32 torque = (point - m_Position).Cross(impulse);
			m_AngularVelocity += torque / m_MassData.inertia;
		}
	}

	void Body::ApplyAngularImpulse(f32 impulse, bool wake)
	{
		if (m_Type != BodyType::Dynamic)
			return;

		if (wake)
			SetAwake(true);

		//Only apply impulse when awake
		if (m_Awake)
		{
			m_AngularVelocity += impulse / m_MassData.inertia;
		}
	}

	void Body::UpdateAABB()
	{
		if (!m_pShape)
		{
			m_AABB.min = m_AABB.max = f32v2::Zero;
			return;
		}
		m_pShape->UpdateAABB();
		m_AABB = m_pShape->m_AABB;
		for (Shape* pShape = m_pShape->m_pNext; pShape; pShape = pShape->m_pNext)
		{
			pShape->UpdateAABB();
			m_AABB.Combine(pShape->m_AABB);
		}
	}

	void Body::SetAwake(bool awake)
	{
		if (m_Awake == awake)
			return;
	}

	void Body::SetActive(bool active)
	{
		m_Active = active;
	}
}
