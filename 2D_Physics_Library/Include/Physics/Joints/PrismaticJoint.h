#pragma once
#include "Joint.h"

namespace P2D {
	
	struct PrismaticJointDef : JointDef
	{
		f32v2 axis;
		f32 tolerance;
		f32 minValue;
		f32 maxValue;
		
		PrismaticJointDef();
	};
	
	class PrismaticJoint : public Joint
	{
	public:
		PrismaticJoint(const PrismaticJointDef& def);
		~PrismaticJoint() override;
		void Update(f32 dt) override;
		bool DoShapesCollide() override;

	private:
		f32v2 m_Axis;
		f32 m_Tolerance;
		f32 m_MinValue;
		f32 m_MaxValue;
	};
}
