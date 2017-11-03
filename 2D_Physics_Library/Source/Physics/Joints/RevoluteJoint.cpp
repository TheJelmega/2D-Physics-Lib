#include "Physics/Joints/RevoluteJoint.h"
#include "Physics/Body.h"

namespace P2D {

	RevoluteJointDef::RevoluteJointDef()
		: limitAngle(false)
		, minAngle(0)
		, maxAngle(0)
		, hasMotor(false)
		, motorSpeed(0)
	{
	}

	RevoluteJoint::RevoluteJoint(const RevoluteJointDef& def)
		: Joint(def)
		, m_LimitAngle(def.limitAngle)
		, m_MinAngle(def.minAngle)
		, m_MaxAngle(def.maxAngle)
		, m_HasMotor(def.hasMotor)
		, m_MotorSpeed(def.motorSpeed)
	{
	}

	RevoluteJoint::~RevoluteJoint()
	{
	}

	void RevoluteJoint::Update(f32 dt)
	{
		(void)dt;
		if (m_HasMotor)
		{
			f32 relAVel;
			if (m_pBody0)
				relAVel = m_pBody1->m_AngularVelocity - m_pBody0->m_AngularVelocity;
			else
				relAVel = m_pBody1->m_AngularVelocity;
			if (relAVel < m_MotorSpeed)
			{
				f32 diff = m_MotorSpeed - relAVel;
				m_pBody1->m_AngularVelocity += diff;
			}
		}
		if (m_LimitAngle)
		{
			f32 curAngle;
			if (m_pBody0)
				curAngle = m_pBody1->m_Angle - m_pBody0->m_Angle;
			else
				curAngle = m_pBody1->m_Angle;
			if (curAngle < m_MinAngle)
			{
				f32v2 point = m_pBody1->m_Transform.Move(m_Pos1);
				//f32 diff = m_MinAngle - curAngle;
				m_pBody1->m_Angle = m_MinAngle + m_pBody0->m_Angle;
				//m_pBody1->m_Position.RotateAroundPoint(point, diff);
				if (m_pBody1->m_AngularVelocity < 0.f)
					m_pBody1->m_AngularVelocity = 0.f;
			}
			if (curAngle > m_MaxAngle)
			{
				f32v2 point = m_pBody1->m_Transform.Move(m_Pos1);
				//f32 diff = curAngle - m_MaxAngle;
				m_pBody1->m_Angle = m_MaxAngle + m_pBody0->m_Angle;
				//m_pBody1->m_Position.RotateAroundPoint(point, diff);
				if (m_pBody1->m_AngularVelocity > 0.f)
					m_pBody1->m_AngularVelocity = 0.f;
			}
		}

		/*f32v2 pos0 = m_pBody0 ? m_pBody0->m_Transform.Move(m_Pos0) : m_Pos0;
		f32v2 pos1 = m_pBody1->m_Transform.Move(m_Pos1);
		f32v2 diff = pos1 - pos0;
		if (m_pBody0)
		{
			f32v2 r0 = pos0 - m_pBody0->m_Position;
			f32v2 r1 = pos1 - m_pBody1->m_Position;
			f32v2 vel0 = m_pBody0->m_LinearVelocity + r0.Cross(m_pBody0->m_AngularVelocity);
			f32v2 vel1 = m_pBody1->m_LinearVelocity + r1.Cross(m_pBody1->m_AngularVelocity);
			f32v2 lvel0 = m_pBody0->m_LinearVelocity;// - r0.Cross(m_pBody0->m_AngularVelocity);
			f32v2 lvel1 = m_pBody1->m_LinearVelocity;// - r1.Cross(m_pBody1->m_AngularVelocity);

			f32 massScalar = m_pBody0->m_MassData.mass / (m_pBody0->m_MassData.mass + m_pBody1->m_MassData.mass);
			f32v2 velDiff = vel1 - vel0;
			f32v2 velDiff0 = velDiff * massScalar;
			f32v2 velDiff1 = velDiff - velDiff0;
			f32v2 lvelDiff = lvel1 - lvel0;
			f32v2 lvelDiff0 = lvelDiff * massScalar;
			f32v2 lvelDiff1 = lvelDiff - lvelDiff0;
			f32v2 diff0 = diff * massScalar;
			f32v2 diff1 = diff - diff0;

			//if (vel0.SqLength() > Math::Epsilon<f32>)
				m_pBody0->m_LinearVelocity += lvelDiff1;
			//if (vel1.SqLength() > Math::Epsilon<f32>)
				m_pBody1->m_LinearVelocity -= lvelDiff0;
			
			f32 avel = 2.f * r1.Cross(velDiff) * m_pBody0->m_MassData.invInertia;
			m_pBody0->m_AngularVelocity -= avel;
			//m_pBody0->m_Position.RotateAroundPoint(pos0, -avel);
			
			avel = 2.f * r0.Cross(velDiff) * m_pBody1->m_MassData.invInertia;
			m_pBody1->m_AngularVelocity -= avel;
			//m_pBody1->m_Position.RotateAroundPoint(pos1, avel);

			m_pBody0->m_Position += diff0;
			m_pBody1->m_Position -= diff1;
		}
		else
		{
			m_pBody1->m_Position -= diff;
		}*/

		f32v2 pos0 = m_pBody0 ? m_pBody0->m_Transform.Move(m_Pos0) : m_Pos0;
		f32v2 pos1 = m_pBody1->m_Transform.Move(m_Pos1);

		f32v2 bp0 = m_pBody0 ? m_pBody0->m_Position : pos0;
		f32v2 bp1 = m_pBody1->m_Position;

		f32v2 target;
		if (m_pBody0 && m_pBody0->m_Type != BodyType::Static)
			target = (pos1 + pos0) * .5f;
		else
			target = pos0;

		if (m_pBody0 && m_pBody0->m_Type != BodyType::Static)
		{
			f32v2 r0 = pos0 - bp0;
			f32v2 r0t = target - bp0;

			f32 angle0 = r0.Angle(r0t);
			m_pBody0->m_Angle += angle0;
			m_pBody0->m_AngularVelocity += angle0 * m_pBody0->m_MassData.invInertia;
			pos0 = m_pBody0->m_Transform.Move(m_Pos0);
			f32v2 diff0 = target - pos0;
			m_pBody0->m_Position += diff0;
			m_pBody0->m_LinearVelocity += diff0;
		}

		f32v2 r1 = pos1 - bp1;
		f32v2 r1t = target - bp1;

		f32 angle1 = r1.Angle(r1t);
		m_pBody1->m_Angle += angle1;
		m_pBody1->m_AngularVelocity += angle1 * m_pBody1->m_MassData.invInertia;
		pos0 = m_pBody1->m_Transform.Move(m_Pos1);
		f32v2 diff1 = target - pos1;
		m_pBody1->m_Position += diff1;
		m_pBody1->m_LinearVelocity += diff1;
		
	}

	bool RevoluteJoint::DoShapesCollide()
	{
		return false;
	}
}
