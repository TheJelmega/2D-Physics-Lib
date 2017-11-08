#pragma once
#include "Joint.h"

namespace P2D {
	
	struct RevoluteJointDef : JointDef
	{
		bool limitAngle;	/**< Whether to limit the angle*/
		f32 minAngle;		/**< Min angle limit*/
		f32 maxAngle;		/**< Max angle limit*/

		bool hasMotor;		/**< Whether th joint has a motor*/
		f32 motorSpeed;		/**< Motor speed*/

		RevoluteJointDef();
	};

	class RevoluteJoint : public Joint
	{
	public:
		RevoluteJoint(const RevoluteJointDef& def);
		~RevoluteJoint();

		void Update(f32 dt) override;

		bool DoShapesCollide() override;
		
		/**
		 * Set whether to limit the angle
		 * @param[in] limit Whether to limit the angle
		 */
		P2D_FORCE_INL void LimitAngle(bool limit) { m_LimitAngle = limit; }
		/**
		 * Check whether the angle is limited
		 * @return Whether the angle is limited
		 */
		P2D_FORCE_INL bool IsAngleLimited() const { return m_LimitAngle; }
		/**
		 * Set the angle limits
		 * @param[in] min	Min angle
		 * @param[in] max	Max angle
		 */
		P2D_FORCE_INL void SetAngleLimits(f32 min, f32 max) { m_MinAngle = min; m_MaxAngle = max; }
		/**
		 * Set the min angle limit
		 * @param[in] angle Min angle
		 */
		P2D_FORCE_INL void SetMinAngle(f32 angle) { m_MinAngle = angle; }
		/**
		 * Get the min angle
		 * @return	Min angle
		 */
		P2D_FORCE_INL f32 GetMinAngle() const { return m_MinAngle; }
		/**
		 * Set the min angle limit
		 * @param[in] angle Max angle
		 */
		P2D_FORCE_INL void SetMaxAngle(f32 angle) { m_MaxAngle = angle; }
		/**
		 * Get the max angle
		 * @return	Max angle
		 */
		P2D_FORCE_INL f32 GetMaxAngle() const { return m_MaxAngle; }

		/**
		 * Set whether a motor speed is applied
		 * @param[in] hasMotor Whether a motor speed is applied
		 */
		P2D_FORCE_INL void SetMotor(bool hasMotor) { m_HasMotor = hasMotor; }
		/**
		 * Check whether a motor speed is applied
		 * @return	Whether a motor speed is applied
		 */
		P2D_FORCE_INL bool HasMotor() const { return m_HasMotor; }
		/**
		 * Set the motor speed
		 * @param[in] speed	Motor speed
		 */
		P2D_FORCE_INL void SetMotorSpeed(f32 speed) { m_MotorSpeed = speed; }
		/**
		 * Get the motor speed
		 * @return	Motor speed
		 */
		P2D_FORCE_INL f32 GetMotorSpeed() const { return m_MotorSpeed; }
		

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
