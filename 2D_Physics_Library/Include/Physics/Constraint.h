#pragma once
#include "Math/Math.h"

namespace P2D {

	class Body;

	struct ConstraintDef
	{
		bool constrainRotation; /**< Whether to constraint rotation*/
		bool constrainPosition; /**< Whether to constrain position*/
		Body* pBody; /**< Body to constrain*/

		f32 minAngle; /**< Min angle*/
		f32 maxAngle; /**< Max angle*/

		f32v2 axis; /** < Axis to constrain the position to (constraining axis)*/
		f32v2 axisPosition; /**< Position of the axis*/
		f32 axisTolerance; /**< Tolerance perpendicular to the axis, large tolerances can constrain the axis to a box*/
		f32 axisMinValue; /**< Min value on the axis*/
		f32 axisMaxValue; /**< max value on the axis*/

		ConstraintDef();
	};
	
	class Constraint
	{
	public:
		Constraint(const ConstraintDef& def);
		~Constraint();

		/**Update the constraint (for internal use)*/
		void Update();

		/**
		 * Set whether rotation should be constrained
		 * @param[in] constrain Whether rotation should be constrained
		 */
		void SetConstrainRotation(bool constrain) { m_ConstrainRotation = constrain; }
		/**
		 * Check whether rotation is constrained
		 * @return Whether rotation is constrained
		 */
		bool IsRotationConstrained() const { return m_ConstrainRotation; }
		/**
		 * Set whether position should be constrained
		 * @param[in] constrain Whether position should be constrained
		 */
		void SetConstrainPosition(bool constrain) { m_ConstrainRotation = constrain; }
		/**
		* Check whether position is constrained
		* @return Whether position is constrained
		*/
		bool IsPositionConstrained() const { return m_ConstrainRotation; }

		/**
		 * Set the angle limits
		 * @param[in] min	Min angle
		 * @param[in] max	Max angle
		 */
		void SetAngleLimits(f32 min, f32 max) { m_MinAngle = min; m_MaxAngle = max; }
		/**
		 * Set the min angle
		 * @param[in] angle	Angle
		 */
		void SetMinAngle(f32 angle) { m_MinAngle = angle; }
		/**
		 * Get the min angle
		 * @return	Min angle
		 */
		f32 GetMinAngle() const { return m_MinAngle; }
		/**
		 * Set the max angle
		 * @param[in] angle	Angle
		 */
		void SetMaxAngle(f32 angle) { m_MaxAngle = angle; }
		/**
		 * Get the max angle
		 * @return	Max angle
		 */
		f32 GetMaxAngle() const { return m_MaxAngle; }

		/**
		 * Set the constraining axis
		 * @param[in] axis	Constraining axis
		 */
		void SetConstrainingAxis(const f32v2& axis) { m_Axis = axis; }
		/**
		 * Get the constraining axis
		 * @return Constraining axis
		 */
		const f32v2& GetConstrainingAxis() const { return m_Axis; }
		/**
		 * Set the axis position
		 * @param[in] position	Axis position
		 */
		void SetAxisPosition(const f32v2& position) { m_AxisPos = position; }
		/**
		 * Get the axis position
		 * @return Axis position
		 */
		const f32v2& GetAxisPosition() const { return m_AxisPos; }
		/**
		 * Set the axis tolerance
		 * @param[in] tolerance	Tolerance
		 */
		void SetAxisTolerance(f32 tolerance) { m_AxisTolerance = tolerance; }
		/**
		 * Get the axis tolerance
		 * @return Axis tolerance
		 */
		f32 GetAxisTolerance() const { return m_AxisTolerance; }
		/**
		 * Set the axis limits
		 * @param[in] min	Min limit
		 * @param[in] max	Max limit
		 */
		void SetAxisLimits(f32 min, f32 max) { m_AxisMin = min; m_AxisMax = max; }
		/**
		 * Set the min axis limit
		 * @param[in] min	Min limit
		 */
		void SetMinAxisLimit(f32 min) { m_AxisMin = min; }
		/**
		 * Get the axis min limit
		 * @return Axis min limit
		 */
		f32 GetMinAxisLimit() const { return m_AxisMin; }
		/**
		 * Set the max axis limit
		 * @param[in] max	Max limit
		 */
		void SetMaxAxisLimit(f32 max) { m_AxisMax = max; }
		/**
		 * Get the axis min limit
		 * @return Axis max limit
		 */
		f32 GetMaxAxisLimit() const { return m_AxisMax; }

	private:
		friend class World;
		friend class PhysicsSolver;

		Body* m_pBody;
		Constraint* m_pNext;
		Constraint* m_pPrev;

		bool m_ConstrainRotation;
		bool m_ConstrainPosition;
		f32 m_MinAngle;
		f32 m_MaxAngle;
		f32v2 m_Axis;
		f32v2 m_AxisPos;
		f32 m_AxisTolerance;
		f32 m_AxisMin;
		f32 m_AxisMax;
	};

}