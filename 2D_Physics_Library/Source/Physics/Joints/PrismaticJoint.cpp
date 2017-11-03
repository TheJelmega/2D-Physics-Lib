#include "Physics/Joints/PrismaticJoint.h"
#include "Physics/Body.h"

namespace P2D {


	PrismaticJointDef::PrismaticJointDef()
		: axis(f32v2::Up)
		, tolerance(Math::Epsilon<f32>)
		, minValue(std::numeric_limits<f32>::lowest())
		, maxValue(std::numeric_limits<f32>::max())
	{
	}

	PrismaticJoint::PrismaticJoint(const PrismaticJointDef& def)
		: Joint(def)
		, m_Axis(def.axis)
		, m_Tolerance(def.tolerance)
		, m_MinValue(def.minValue)
		, m_MaxValue(def.maxValue)
	{
	}

	PrismaticJoint::~PrismaticJoint()
	{
	}

	void PrismaticJoint::Update(f32 dt)
	{
		(void)dt;
		f32v2 pos0 = m_pBody0 ? m_pBody0->m_Transform.Move(m_Pos0) : m_Pos0;
		f32v2 pos1 = m_pBody1->m_Transform.Move(m_Pos1);

		f32v2 p0p1 = pos1 - pos0;
		f32v2 axis;
		if (m_pBody0)
			axis = m_Axis.Rotated(m_pBody0->m_Angle);
		else
			axis = m_Axis;
		f32 dot = axis.Dot(p0p1);

		dot = Math::Clamp(dot, m_MinValue, m_MaxValue);
		f32v2 closestPoint = pos0 + axis * dot;

		f32v2 diff = pos1 - closestPoint;
		f32 l = diff.Length();

		if (l > m_Tolerance)
		{
			f32v2 r1 = pos1 - m_pBody1->m_Position;
			f32v2 r1t = closestPoint - m_pBody1->m_Position;

			f32 angle1 = r1.Angle(r1t);
			m_pBody1->m_Angle += angle1;
			m_pBody1->m_AngularVelocity += angle1 * m_pBody1->m_MassData.invInertia;
			pos1 = m_pBody1->m_Transform.Move(m_Pos1);

			diff = pos1 - closestPoint;
			l = diff.Length();

			diff /= l;
			l -= m_Tolerance;
			diff *= l;

			m_pBody1->m_Position -= diff;
			m_pBody1->m_LinearVelocity -= diff;
		}

	}

	bool PrismaticJoint::DoShapesCollide()
	{
		return true;
	}
}
