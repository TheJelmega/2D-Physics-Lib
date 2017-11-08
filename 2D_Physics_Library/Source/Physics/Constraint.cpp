#include "Physics/Constraint.h"
#include "Physics/Body.h"

namespace P2D {


	ConstraintDef::ConstraintDef()
		: constrainRotation(false)
		, constrainPosition(false)
		, pBody(nullptr)
		, minAngle(0)
		, maxAngle(0)
		, axisTolerance(0)
		, axisMinValue(std::numeric_limits<f32>::lowest())
		, axisMaxValue((std::numeric_limits<f32>::max)())
	{
	}

	Constraint::Constraint(const ConstraintDef& def)
		: m_pBody(def.pBody)
		, m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_ConstrainRotation(def.constrainRotation)
		, m_ConstrainPosition(def.constrainPosition)
		, m_MinAngle(def.minAngle)
		, m_MaxAngle(def.maxAngle)
		, m_Axis(def.axis.Normalized())
		, m_AxisPos(def.axisPosition)
		, m_AxisTolerance(def.axisTolerance)
		, m_AxisMin(def.axisMinValue)
		, m_AxisMax(def.axisMaxValue)
	{
	}

	Constraint::~Constraint()
	{
	}

	void Constraint::Update()
	{
		if (m_ConstrainRotation)
		{
			f32 angle = m_pBody->m_Angle;
			if (angle > m_MaxAngle || angle < m_MinAngle)
			{
				f32 correction;
				if (angle > m_MaxAngle)
					correction = m_MaxAngle - angle;
				else
					correction = m_MinAngle - angle;

				m_pBody->m_Angle += correction;
				f32v2 comLoc = m_pBody->m_Transform.Move(m_pBody->m_MassData.centerOfMass);
				m_pBody->m_Position.RotateAroundPoint(comLoc, correction);

				if (angle > m_MaxAngle && m_pBody->m_AngularVelocity > 0.f)
					m_pBody->m_AngularVelocity = 0.f;
				else if (angle < m_MinAngle && m_pBody->m_AngularVelocity < 0.f)
					m_pBody->m_AngularVelocity = 0.f;
			}
		}

		if (m_ConstrainPosition)
		{
			f32v2 pos = m_pBody->m_Position;
			f32 dot = m_Axis.Dot(pos - m_AxisPos);
			f32 u = Math::Clamp(dot, m_AxisMin, m_AxisMax);
			f32v2 closestPos = m_AxisPos + m_Axis * u;
			f32 maxDist = m_AxisTolerance * m_AxisTolerance;
			f32v2 dir = pos - closestPos;
			f32 dist = dir.SqLength();

			if (dist > maxDist)
			{
				//dir /= sqrt(dist);
				f32 extraVel = sqrt(dist);
				f32v2 temp = dir;
				dir /= extraVel;
				m_pBody->m_Position = closestPos + dir * m_AxisTolerance;
				dir = temp;

				/*f32 speed = m_pBody->m_LinearVelocity.Dot(dir);
				m_pBody->m_LinearVelocity -= dir * speed;*/
				f32v2 vdir = m_Axis;
				f32v2 hdir = m_Axis.Cross(1.f);
				vdir.Normalize();
				hdir.Normalize();

				if (dot > 0 && u < 0 || dot < 0 && u > dot)
				{
					f32 speed = m_pBody->m_LinearVelocity.Dot(vdir);// -dir.Dot(vdir);
					m_pBody->m_LinearVelocity -= vdir * speed;// *2.f;
				}
				if (abs(hdir.Dot(dir)) > m_AxisTolerance)
				{
					f32 speed = m_pBody->m_LinearVelocity.Dot(hdir) + dir.Dot(hdir);
					m_pBody->m_LinearVelocity -= hdir * speed;// *2.f;
				}
			}
			
		}
	}
}
