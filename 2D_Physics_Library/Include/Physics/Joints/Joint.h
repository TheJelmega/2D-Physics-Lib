#pragma once
#include "Math/Math.h"

namespace P2D {

	class Joint;
	class Body;
	
	struct JointDef
	{
		Body* pBody0; /**< Body 0, if nullptr, the joint is connected to the world*/
		Body* pBody1; /**< Body 1*/
		f32v2 pos0; /**< Relative position to body 0/world*/
		f32v2 pos1; /**< Relative position to body 1*/
	};

	struct JointNode
	{
		Joint* pJoint; /**< Joint*/
		Body* pOther; /**< Other body, nullptr if connected to world*/
		JointNode* pNext; /**< Next node*/
		JointNode* pPrev; /**< Previous node*/
	};

	class Joint
	{
	public:
		Joint(const JointDef& def);
		virtual ~Joint();

		/**
		 * Update the joint (used internally)
		 */
		virtual void Update(f32 dt);

		/**
		 * Check whether shapes of the parent bodies should collide
		 * @return Whether shape should collide
		 */
		virtual bool DoShapesCollide();

		/**
		 * Get relative position to body 0/world
		 * @return Relative position to body 0/world
		 */
		const f32v2& GetPos0() const { return m_Pos0; }
		/**
		 * Get relative position to body 1
		 * @return Relative position to body 1
		 */
		const f32v2& GetPos1() const { return m_Pos1; }

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