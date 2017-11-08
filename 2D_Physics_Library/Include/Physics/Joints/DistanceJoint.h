#pragma once
#include "Joint.h"

namespace P2D {
	
	struct DistanceJointDef : JointDef
	{
		f32 distance;	/**< Distance between points*/
		f32 tolerance;	/**< Distance tolerance*/

		DistanceJointDef();
	};

	class DistanceJoint : Joint
	{
	public:
		DistanceJoint(const DistanceJointDef& def);
		~DistanceJoint() override;

		void Update(f32 dt) override;
		bool DoShapesCollide() override;

		/**
		 * Set the distance of the joint
		 * @param[in] distance	Distance
		 */
		P2D_FORCE_INL void SetDistance(f32 distance) { m_Distance = distance; }
		/**
		 * Get the distance of the joint
		 * @return	Joint distance
		 */
		P2D_FORCE_INL f32 GetDistance() const { return m_Distance; }
		/**
		 * Set the tolerance of the joint
		 * @param[in] tolerance	Tolerance
		 */
		P2D_FORCE_INL void SetTolerance(f32 tolerance) { m_Tolerance = tolerance; }
		/**
		 * Get the tolerance of the joint
		 * @return	Joint tolerance
		 */
		P2D_FORCE_INL f32 GetTolerance() const { return m_Tolerance; }
		

	private:
		friend class World;

		f32 m_Distance;
		f32 m_Tolerance;
	};

}
