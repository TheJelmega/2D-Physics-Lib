#include "Physics/Joints/DistanceJoint.h"
#include "Physics/Body.h"

namespace P2D {


	DistanceJointDef::DistanceJointDef()
		: distance(0)
	    , tolerance(Math::Epsilon<f32>)
	{
	}

	DistanceJoint::DistanceJoint(const DistanceJointDef& def)
		: Joint(def)
		, m_Distance(def.distance)
		, m_Tolerance(def.tolerance)
	{
	}

	DistanceJoint::~DistanceJoint()
	{
	}

	void DistanceJoint::Update(f32 dt)
	{
		(void)dt;
		f32v2 pos0 = m_pBody0 ? m_pBody0->m_Transform.Move(m_Pos0) : m_Pos0;
		f32v2 pos1 = m_pBody1->m_Transform.Move(m_Pos1);

		f32v2 diff = pos1 - pos0;
		f32 l = diff.Length();

		if (l < m_Distance - m_Tolerance || l > m_Distance + m_Tolerance)
		{
			if (m_pBody0 || m_pBody0->m_Type != BodyType::Static)
			{
				f32v2 orig = diff * .5f;
				f32 hl = Math::Clamp(l, m_Distance - m_Tolerance, m_Distance + m_Tolerance) * .5f;
				diff /= l;
				diff *= hl;
				diff = orig - diff;
				m_pBody0->m_Position += diff;
				m_pBody0->m_LinearVelocity += diff;
				m_pBody1->m_Position -= diff;
				m_pBody1->m_LinearVelocity -= diff;
			}
			else
			{
				f32v2 orig = diff;
				diff /= l;
				l = Math::Clamp(l, m_Distance - m_Tolerance, m_Distance + m_Tolerance);
				diff *= l;
				diff = orig - diff;
				m_pBody1->m_Position += diff;
				m_pBody1->m_LinearVelocity += diff;
			}
		}
	}

	bool DistanceJoint::DoShapesCollide()
	{
		return true;
	}
}
