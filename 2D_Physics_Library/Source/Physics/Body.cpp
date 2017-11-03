#include "Physics/Body.h"
#include "Physics/World.h"
#include "Physics/BroadPhase.h"

namespace P2D {

	BodyMassData::BodyMassData()
		: mass(0)
		, invMass(0)
		, inertia(0)
		, invInertia(0)
	{
	}

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
		, m_pJointList(nullptr)
		, m_Type(def.type)
		, m_Position(def.position)
		, m_Angle(def.angle)
		, m_Velocity()
		, m_Torque(0.f)
		, m_SolverIndex(-1)
		, m_SleepTimer(0)
		, m_Active(def.active)
		, m_Awake(def.awake)
		, m_InSolver(false)
	{
		if (m_Type == BodyType::Static)
		{
			m_MassData.mass = g_StaticMass;
			m_MassData.invMass = 0.f;
			m_MassData.inertia = g_StaticInertia;
			m_MassData.invInertia = 0.f;
		}
	}

	Body::~Body()
	{
	}

	void Body::AddShape(Shape* pShape)
	{
		if (pShape->m_Type == Shape::Type::Edge || pShape->m_Type == Shape::Type::Chain)
		{
			P2D_ASSERT(m_Type == BodyType::Static);
		}

		if (m_pShape)
			pShape->m_pNext = m_pShape;
		m_pShape = pShape;
		pShape->m_pBody = this;
		pShape->UpdateAABB();
		if (m_ShapeCount > 0)
			m_AABB.Combine(pShape->m_AABB);
		else
			m_AABB = pShape->m_AABB;
		++m_ShapeCount;

		//Update mass data, if not static
		if (m_Type != BodyType::Static)
		{
			f32v2 tempCenterOfMass = m_MassData.mass * m_MassData.centerOfMass + pShape->m_MassData.mass * pShape->m_MassData.centerOfMass;
			m_MassData.mass += pShape->m_MassData.mass;
			if (m_MassData.mass < Math::Epsilon<f32>)
				m_MassData.invMass = 0.f;
			else
				m_MassData.invMass = 1.f / m_MassData.mass;
			m_MassData.centerOfMass = tempCenterOfMass / m_MassData.mass;
			/*pShape->UpdateInertia();
			m_MassData.inertia += pShape->m_MassData.inertia;
			if (m_MassData.inertia < Math::Epsilon<f32>)
				m_MassData.invInertia = 0.f;
			else
				m_MassData.invInertia = 1.f / m_MassData.inertia;*/
		

			//Recalculate inertia
			for (Shape* pShape0 = m_pShape; pShape0; pShape0 = pShape0->m_pNext)
			{
				pShape0->UpdateInertia();
				m_MassData.inertia += pShape0->m_MassData.inertia;
			}
			if (m_MassData.inertia < Math::Epsilon<f32>)
				m_MassData.invInertia = 0.f;
			else
				m_MassData.invInertia = 1.f / m_MassData.inertia;
		}

		m_pWorld->m_pBroadPhase->AddShape(pShape);
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
			m_Torque += (point - (m_Position + m_MassData.centerOfMass)).Cross(force);
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
			m_LinearVelocity += impulse * m_MassData.invMass;
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
			m_LinearVelocity += impulse * m_MassData.invMass;
			f32 torque = (point - (m_Position + m_MassData.centerOfMass)).Cross(impulse);
			m_AngularVelocity += torque * m_MassData.invInertia;
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
			m_AngularVelocity += impulse * m_MassData.invInertia;
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

		m_Awake = awake;
		m_SleepTimer = 0.f;
	}

	void Body::SetActive(bool active)
	{
		m_Active = active;
		if (active)
			SetAwake(true);
	}
}
