#pragma once
#include "Joint.h"

namespace P2D {
	
	struct PrismaticJointDef : JointDef
	{
		f32v2 axis;		/**< Sliding axis*/
		f32 tolerance;	/**< Axis tolerance*/
		f32 minValue;	/**< Min limit*/
		f32 maxValue;	/**< Max limit*/
		
		PrismaticJointDef();
	};
	
	class PrismaticJoint : public Joint
	{
	public:
		PrismaticJoint(const PrismaticJointDef& def);
		~PrismaticJoint() override;
		void Update(f32 dt) override;
		bool DoShapesCollide() override;

		/**
		 * Set the sliding axis of the joint
		 * @param[in] axis	Sliding axis
		 */
		P2D_FORCE_INL void SetAxis(const f32v2& axis) { m_Axis = axis; }
		/**
		 * Get the sliding axis of the joint
		 * @return	Sliding axis
		 */
		P2D_FORCE_INL const f32v2& GetAxis() const { return m_Axis; }
		/**
		 * Set the tolerance of the joint
		 * @param[in] tolerance	Tolerance
		 */
		P2D_FORCE_INL void SetTolerance(f32 tolerance) { m_Tolerance = tolerance; }
		/**
		 * Get the tolerance of the joint
		 * @return	Tolerance
		 */
		P2D_FORCE_INL f32 GetTolerance() const { return m_Tolerance; }
		/**
		* Set the tolerance of the joint
		* @param[in] min	Min limit
		* @param[in] max	max limit
		*/
		P2D_FORCE_INL void SetLimits(f32 min, f32 max) { m_MinValue = min; m_MaxValue = max; }
		/**
		 * Set the min limit of the joint
		 * @param[in] min	Min limit
		 */
		P2D_FORCE_INL void SetMinLimit(f32 min) { m_MinValue = min; }
		/**
		 * Get the min limit of the joint
		 * @return	Min limit
		 */
		P2D_FORCE_INL f32 GetMinLimit() const { return m_MinValue; }
		/**
		 * Set the max limit of the joint
		 * @param[in] max	max limit
		 */
		P2D_FORCE_INL void SetMaxLimit(f32 max) { m_MaxValue = max; }
		/**
		 * Get the max limit of the joint
		 * @return	Max limit
		 */
		P2D_FORCE_INL f32 GetMaxlimit() const { return m_MaxValue; }

	private:
		f32v2 m_Axis;
		f32 m_Tolerance;
		f32 m_MinValue;
		f32 m_MaxValue;
	};
}
