#include "Physics/Joints/FixedJoint.h"
#include "Physics/Body.h"

namespace P2D {


	FixedJointDef::FixedJointDef()
		: angle(0)
	{
	}

	FixedJoint::FixedJoint(const FixedJointDef& def)
		: Joint(def)
		, m_Angle(def.angle)
	{
	}

	FixedJoint::~FixedJoint()
	{
	}

	void FixedJoint::Update(f32 dt)
	{
		(void)dt;
		f32v2 pos0 = m_pBody0 ? m_pBody0->m_Transform.Move(m_Pos0) : m_Pos0;
		f32v2 pos1 = m_pBody1->m_Transform.Move(m_Pos1);

		f32v2 diff = pos1 - pos0;
		if (m_pBody0 && m_pBody0->m_Type != BodyType::Static)
		{
			diff *= .5f;
			m_pBody0->m_Position += diff;
			m_pBody1->m_Position -= diff;
			f32 angleDiff = m_pBody1->m_Angle - m_pBody0->m_Angle;
			angleDiff = (angleDiff - m_Angle) * .5f;
			m_pBody0->m_Angle += angleDiff;
			m_pBody1->m_Angle -= angleDiff;
			f32v2 lvel = (m_pBody0->m_LinearVelocity + m_pBody1->m_LinearVelocity) * .5f;
			m_pBody0->m_LinearVelocity = lvel;
			m_pBody1->m_LinearVelocity = lvel;
			f32 avel = (m_pBody0->m_AngularVelocity + m_pBody1->m_AngularVelocity) * .5f;
			m_pBody0->m_AngularVelocity = avel;
			m_pBody1->m_AngularVelocity = avel;
		}
		else
		{
			m_pBody1->m_Position -= diff;
			m_pBody1->m_Angle = m_pBody0 ? m_Angle - m_pBody0->m_Angle : m_Angle;
			m_pBody1->m_LinearVelocity = f32v2::Zero;
			m_pBody0->m_AngularVelocity = 0.f;
		}
	}

	bool FixedJoint::DoShapesCollide()
	{
		return false;
	}
}
