#pragma once
#include "Joint.h"

namespace P2D {
	
	struct DistanceJointDef : JointDef
	{
		f32 distance;
		f32 tolerance;

		DistanceJointDef();
	};

	class DistanceJoint : Joint
	{
	public:
		DistanceJoint(const DistanceJointDef& def);
		~DistanceJoint() override;

		void Update(f32 dt) override;
		bool DoShapesCollide() override;

		P2D_FORCE_INL f32 GetDistance() const { return m_Distance; }
		P2D_FORCE_INL void SetDistance(f32 distance) { m_Distance = distance; }
		P2D_FORCE_INL f32 GetTolerance() const { return m_Tolerance; }
		P2D_FORCE_INL void SetTolerance(f32 tolerance) { m_Tolerance = tolerance; }

	private:
		friend class World;

		f32 m_Distance;
		f32 m_Tolerance;
	};

}
