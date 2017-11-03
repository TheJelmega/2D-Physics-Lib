#include "Physics/Joints/Joint.h"
#include "Physics/Body.h"

namespace P2D {


	Joint::Joint(const JointDef& def)
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pBody0(def.pBody0)
		, m_pBody1(def.pBody1)
		, m_Pos0(def.pos0)
		, m_Pos1(def.pos1)
	{
		m_Node0.pJoint = this;
		m_Node0.pOther = def.pBody1;
		m_Node1.pJoint = this;
		m_Node1.pOther = def.pBody0;
	}

	Joint::~Joint()
	{
	}

	void Joint::Update(f32 dt)
	{
		(void)dt;
		//Default joint does nothing
	}

	bool Joint::DoShapesCollide()
	{
		return true;
	}
}
