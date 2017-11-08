#pragma once
#include "Joint.h"

namespace P2D {
	
	struct FixedJointDef : JointDef
	{
		f32 angle; /**Angle between 2 bodies*/

		FixedJointDef();
	};

	class FixedJoint : public Joint
	{
	public:
		FixedJoint(const FixedJointDef& def);
		~FixedJoint() override;
		void Update(f32 dt) override;
		bool DoShapesCollide() override;

	private:
		friend class World;

		f32 m_Angle;
	};

}
