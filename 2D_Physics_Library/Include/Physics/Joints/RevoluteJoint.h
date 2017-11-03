#pragma once
#include "Joint.h"

namespace P2D {
	
	struct RevoluteJointDef : JointDef
	{
		bool limitAngle;
		f32 minAngle;
		f32 maxAngle;

		bool hasMotor;
		f32 motorSpeed;

		RevoluteJointDef();
	};

	class RevoluteJoint : public Joint
	{
	public:
		RevoluteJoint(const RevoluteJointDef& def);
		~RevoluteJoint();

		void Update(f32 dt) override;

		bool DoShapesCollide() override;

		P2D_FORCE_INL bool IsAngleLimited() const { return m_LimitAngle; }
		P2D_FORCE_INL void LimitAngle(bool limit) { m_LimitAngle = limit; }
		P2D_FORCE_INL f32 GetMinAngle() const { return m_MinAngle; }
		P2D_FORCE_INL void SetMinAngle(f32 angle) { m_MinAngle = angle; }
		P2D_FORCE_INL f32 GetMaxAngle() const { return m_MaxAngle; }
		P2D_FORCE_INL void SetMaxAngle(f32 angle) { m_MaxAngle = angle; }
		P2D_FORCE_INL void SetAngleLimits(f32 min, f32 max) { m_MinAngle = min; m_MaxAngle = max; }

		P2D_FORCE_INL bool HasMotor() const { return m_HasMotor; }
		P2D_FORCE_INL void SetMotor(bool hasMotor) { m_HasMotor = hasMotor; }
		P2D_FORCE_INL f32 GetMotorSpeed() const { return m_MotorSpeed; }
		P2D_FORCE_INL void SetMotorSpeed(f32 speed) { m_MotorSpeed = speed; }

	private:
		friend class World;
		friend class Body;

		bool m_LimitAngle;
		f32 m_MinAngle;
		f32 m_MaxAngle;

		bool m_HasMotor;
		f32 m_MotorSpeed;
	};
}
