#pragma once
#include "Math/Math.h"

namespace P2D {

	class Joint;
	class Body;
	
	struct JointDef
	{
		Body* pBody0;
		Body* pBody1;
		f32v2 pos0;
		f32v2 pos1;
	};

	struct JointNode
	{
		Joint* pJoint;
		Body* pOther;
		JointNode* pNext;
		JointNode* pPrev;
	};

	class Joint
	{
	public:
		Joint(const JointDef& def);
		virtual ~Joint();

		virtual void Update(f32 dt);

		virtual bool DoShapesCollide();

	protected:
		friend class World;
		friend class PhysicsSolver;
		friend class ContactManager;

		Joint* m_pNext;
		Joint* m_pPrev;

		JointNode m_Node0;
		JointNode m_Node1;

		Body* m_pBody0;
		Body* m_pBody1;
		f32v2 m_Pos0;
		f32v2 m_Pos1;
	};

}