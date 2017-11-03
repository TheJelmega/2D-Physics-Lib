#pragma once
#include "Common/BlockAllocator.h"
#include "Body.h"
#include "Shapes/CircleShape.h"
#include "Shapes/EdgeShape.h"
#include "Shapes/ChainShape.h"
#include "EventListener.h"
#include "Contacts/ContactManager.h"
#include "Contacts/ContactFilter.h"
#include "Common/StackAllocator.h"
#include "PhysicsSolver.h"
#include "Shapes/PolygonShape.h"
#include "Constraint.h"
#include "Joints/Joint.h"
#include "Joints/RevoluteJoint.h"
#include "Joints/DistanceJoint.h"
#include "Joints/FixedJoint.h"
#include "Joints/PrismaticJoint.h"
#include "Raycaster.h"

namespace P2D {

	/**
	 * Physics manager
	 */
	class World
	{
	public:
		World();
		~World();

		Body* CreateBody(const BodyDef& def);
		void DestroyBody(Body* pBody);

		CircleShape* CreateShape(const CircleShapeDef& def);
		void DestroyShape(CircleShape* pShape);
		EdgeShape* CreateShape(const EdgeShapeDef& def);
		void DestroyShape(EdgeShape* pShape);
		ChainShape* CreateShape(const ChainShapeDef& def);
		void DestroyShape(ChainShape* pShape);
		PolygonShape* CreateShape(const PolygonShapeDef& def);
		void DestroyShape(PolygonShape* pShape);

		Constraint* CreateConstraint(const ConstraintDef& def);
		void DestroyConstraint(Constraint* pConstraint);

		RevoluteJoint* CreateJoint(const RevoluteJointDef& def);
		void DestroyJoint(RevoluteJoint* pJoint);
		DistanceJoint* CreateJoint(const DistanceJointDef& def);
		void DestroyJoint(DistanceJoint* pJoint);
		FixedJoint* CreateJoint(const FixedJointDef& def);
		void DestroyJoint(FixedJoint* pJoint);
		PrismaticJoint* CreateJoint(const PrismaticJointDef& def);
		void DestroyJoint(PrismaticJoint* pJoint);

		/**
		 * Update all physics, does timesteps, ...
		 */
		void Update(f32 dt);

		/**
		 *	Manually step through 1 iteration of the simulation, you can repeat this until enough time has passed for your current frame
		 *	@param[in] timestep	Fixed timestep (e.g. 1/30 or 1/60), try to refrain from using bigger timesteps then 1/30, this could cause incorrect results
		 */
		void Step(f32 timestep);

		/**
		 * Manually clear all forces on bodies
		 */
		void ClearBodyForces();


		void Raycast(const RaycastInput& input, RaycastOutput& output);

		P2D_FORCE_INL Body* GetBodyList() { return m_pBodyList; }
		P2D_FORCE_INL const Body* GetBodyList() const { return m_pBodyList; }

		P2D_FORCE_INL const Contact* GetTouchingContactList() const { return m_pPhysicsSolver->m_pTouchingContact; }

		P2D_FORCE_INL void SetTimeStep(f32 timestep) { m_Timestep = timestep; }
		P2D_FORCE_INL f32 GetTimeStep() const { return m_Timestep; }

		P2D_FORCE_INL void SetGravity(const f32v2& gravity) { m_Gravity = gravity; }
		P2D_FORCE_INL f32v2 GetGravity() const { return m_Gravity; }

		P2D_FORCE_INL void SetAutoClearForces(bool autoClear) { m_AutoClearForces = autoClear; }

		P2D_FORCE_INL void SetContactFilter(const ContactFilter& filter) { m_ContactFilter = filter; }
		P2D_FORCE_INL void ResetContactFilter() { m_ContactFilter = ContactFilter(); }

		P2D_FORCE_INL EventListener& GetEventListener() { return m_EventListener; }

		//Counts
		P2D_FORCE_INL u32 GetBodyCount() const { return m_BodyCount; }
		P2D_FORCE_INL u32 GetShapeCount() const { return m_ShapeCount; }
		P2D_FORCE_INL u32 GetConstraintCount() const { return m_ConstraintCount; }
		P2D_FORCE_INL u32 GetJointCount() const { return m_JointCount; }
		P2D_FORCE_INL u32 GetContactCount() const { return m_pPhysicsSolver->m_pContactManager->m_ContactCount; }
		P2D_FORCE_INL u32 GetTouchingContactCount() const { return m_pPhysicsSolver->m_ContactCount; }

	private:
		friend class Body;
		friend class Shape;
		friend class PhysicsSolver;
		friend class BroadPhase;
		friend class ContactManager;
		friend struct Collision;
		friend class Raycaster;

		f32 m_Dt;
		f32 m_Timestep;
		bool m_AutoClearForces;

		f32v2 m_Gravity;
		
		Body* m_pBodyList;
		u32 m_BodyCount;
		u32 m_ShapeCount;

		Constraint* m_pConstraintList;
		u32 m_ConstraintCount;

		Joint* m_pJointList;
		u32 m_JointCount;

		BlockAllocator m_Allocator;
		StackAllocator m_StackAllocator;

		EventListener m_EventListener;
		PhysicsSolver* m_pPhysicsSolver;
		ContactFilter m_ContactFilter;

		BroadPhase* m_pBroadPhase;
		Raycaster* m_pRaycaster;
	};

}
