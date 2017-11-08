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

		/**
		 * Create a body
		 * @param[in] def	Body definition
		 * @return			Body created from the definition
		 */
		Body* CreateBody(const BodyDef& def);
		/**
		 * Destroy a body
		 * @param[in] pBody	Body to destroy
		 */
		void DestroyBody(Body* pBody);

		/**
		 * Create a circle shape
		 * @param[in] def	Circle shape definition
		 * @return			Circle shape created from the definition
		 */
		CircleShape* CreateShape(const CircleShapeDef& def);
		/**
		 * Destroy a circle shape
		 * @param[in] pShape	Circle shape to destroy
		 */
		void DestroyShape(CircleShape* pShape);
		/**
		 * Create an edge shape
		 * @param[in] def	Edge shape definition
		 * @return			Edge shape created from the definition
		 */
		EdgeShape* CreateShape(const EdgeShapeDef& def);
		/**
		 * Destroy an edge shape
		 * @param[in] pShape	Edge shape to destroy
		 */
		void DestroyShape(EdgeShape* pShape);
		/**
		 * Create a chain shape
		 * @param[in] def	Chain shape definition
		 * @return			Chain shape created from the definition
		 */
		ChainShape* CreateShape(const ChainShapeDef& def);
		/**
		 * Destroy a chain shape
		 * @param[in] pShape	Chain shape to destroy
		 */
		void DestroyShape(ChainShape* pShape);
		/**
		 * Create a polygon shape
		 * @param[in] def	Polygon shape definition
		 * @return			Polygon shape created from the definition
		 */
		PolygonShape* CreateShape(const PolygonShapeDef& def);
		/**
		 * Destroy a polgon shape
		 * @param[in] pShape Polygon shape to destroy
		 */
		void DestroyShape(PolygonShape* pShape);

		/**
		 * Create a constraint
		 * @param[in] def	Constraint definition
		 * @return			Constraint created from the definition
		 */
		Constraint* CreateConstraint(const ConstraintDef& def);
		/**
		 * Destroy a constraint
		 * @param[in] pConstraint	Constraint to destroy
		 */
		void DestroyConstraint(Constraint* pConstraint);

		/**
		 * Create a revolute joint
		 * @param[in] def	Revolute joint def
		 * @return			Revolute joint created from the definition
		 */
		RevoluteJoint* CreateJoint(const RevoluteJointDef& def);
		/**
		 * Destroy a revolute joint
		 * @param[in] pJoint	Revolute joint to destory
		 */
		void DestroyJoint(RevoluteJoint* pJoint);
		/**
		 * Create a distance joint
		 * @param[in] def	Distance joint def
		 * @return			Distance joint created from the definition
		 */
		DistanceJoint* CreateJoint(const DistanceJointDef& def);
		/**
		 * Destroy a distance joint
		 * @param[in] pJoint	Distance joint to destory
		 */
		void DestroyJoint(DistanceJoint* pJoint);
		/**
		 * Create a fixed joint
		 * @param[in] def	Fixed joint def
		 * @return			Fixed joint created from the definition
		 */
		FixedJoint* CreateJoint(const FixedJointDef& def);
		/**
		 * Destroy a fixed joint
		 * @param[in] pJoint	Fixed joint to destory
		 */
		void DestroyJoint(FixedJoint* pJoint);
		/**
		 * Create a prismatic joint
		 * @param[in] def	Prismatic joint def
		 * @return			Prismatic joint created from the definition
		 */
		PrismaticJoint* CreateJoint(const PrismaticJointDef& def);
		/**
		 * Destroy a prismatic joint
		 * @param[in] pJoint	Prismatic joint to destory
		 */
		void DestroyJoint(PrismaticJoint* pJoint);

		/**
		 * Update all physics over a variable timestep, uses fixed timestep during steps, ...
		 * @param[in] dt	Timestep
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

		/**
		 * Raycast the world
		 * @param[in] input		Raycast input
		 * @param[out] output	Raycast output
		 */
		void Raycast(const RaycastInput& input, RaycastOutput& output);

		/**
		 * Get the list of bodies in the world
		 * @return	First body in the list
		 */
		P2D_FORCE_INL Body* GetBodyList() { return m_pBodyList; }
		P2D_FORCE_INL const Body* GetBodyList() const { return m_pBodyList; }
		/**
		 * Get the amount of bodies in the world
		 * @return	Amount of bodies in the world
		 */
		P2D_FORCE_INL u32 GetBodyCount() const { return m_BodyCount; }
		/**
		* Get the amount of shapes in the world
		* @return	Amount of shapes in the world
		*/
		P2D_FORCE_INL u32 GetShapeCount() const { return m_ShapeCount; }

		/**
		 * Get the list of contacts in the world
		 * @return	First contact in the list
		 */
		P2D_FORCE_INL const Contact* GetContactList() const { return m_pPhysicsSolver->m_pContactManager->m_pContactList; }
		/**
		 * Get the amount of contacts in the world
		 * @return	Amount of contacts in the world
		 */
		P2D_FORCE_INL u32 GetContactCount() const { return m_pPhysicsSolver->m_pContactManager->m_ContactCount; }
		/**
		 * Get the list of touching contacts in the world
		 * @return	First touching contact in the list
		 */
		P2D_FORCE_INL const Contact* GetTouchingContactList() const { return m_pPhysicsSolver->m_pTouchingContact; }
		/**
		 * Get the amount of touching contacts in the world
		 * @return	Amount of touching contacts in the world
		 */
		P2D_FORCE_INL u32 GetTouchingContactCount() const { return m_pPhysicsSolver->m_ContactCount; }

		/**
		 * Get the list of constraints in the world
		 * @return	First constraint in the list
		 */
		P2D_FORCE_INL Constraint* GetConstraintList() { return m_pConstraintList; }
		P2D_FORCE_INL const Constraint* GetConstraintList() const { return m_pConstraintList; }
		/**
		 * Get the amount of constraints in the world
		 * @return	Amount of constraints in the world
		 */
		P2D_FORCE_INL u32 GetConstraintCount() const { return m_ConstraintCount; }

		/**
		 * Get the list of joints in the world
		 * @note	To get the specific joint type, the user needs to store the joint manually, which can be done with the return of the joint create functions
		 * @return	First constraint in the list
		 */
		P2D_FORCE_INL Joint* GetJointList() { return m_pJointList; }
		P2D_FORCE_INL const Joint* GetJointList() const { return m_pJointList; }
		/**
		 * Get the amount of joints in the world
		 * @return	Amount of joints in the world
		 */
		P2D_FORCE_INL u32 GetJointCount() const { return m_JointCount; }

		/**
		 * Set the timestep used during physics updates
		 * @param[in] timestep Timestep to be used during physics updates
		 */
		P2D_FORCE_INL void SetTimeStep(f32 timestep) { m_Timestep = timestep; }
		/**
		 * Get the current timestep used during physics updates
		 * @return	Timestep used during physics updates
		 */
		P2D_FORCE_INL f32 GetTimeStep() const { return m_Timestep; }

		/**
		 * Set the global gravity
		 * @param[in] gravity	Gravity
		 */
		P2D_FORCE_INL void SetGravity(const f32v2& gravity) { m_Gravity = gravity; }
		/**
		 * Get the global gravity
		 * @return	Global gravity
		 */
		P2D_FORCE_INL f32v2 GetGravity() const { return m_Gravity; }

		/**
		 * Set whether to automatically clear forces during update
		 * @note When disabled, it is expected that the user clears the forces manually, using ClearBodyForces();
		 * @param[in] autoClear Whether to automatically clear forces during update
		 */
		P2D_FORCE_INL void SetAutoClearForces(bool autoClear) { m_AutoClearForces = autoClear; }

		/**
		 * Set the contact filter used during collsion filtering
		 * @param[in] filter Contact filter
		 */
		P2D_FORCE_INL void SetContactFilter(const ContactFilter& filter) { m_ContactFilter = filter; }
		/**
		 * Reset the contact filter to the default filter
		 */
		P2D_FORCE_INL void ResetContactFilter() { m_ContactFilter = ContactFilter(); }

		/**
		 * Get a reference to the event listener
		 * @return Reference to the event listener
		 */
		P2D_FORCE_INL EventListener& GetEventListener() { return m_EventListener; }

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
