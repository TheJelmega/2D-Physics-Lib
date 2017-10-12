#pragma once
#include "Common/BlockAllocator.h"
#include "Body.h"
#include "Shapes/CircleShape.h"
#include "EventListener.h"
#include "BroadPhase.h"
#include "Contacts/ContactManager.h"
#include "Contacts/ContactFilter.h"

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


		P2D_FORCE_INL void SetTimeStep(f32 timestep) { m_Timestep = timestep; }
		P2D_FORCE_INL f32 GetTimeStep() const { return m_Timestep; }

		P2D_FORCE_INL void SetGravity(const f32v2& gravity) { m_Gravity = gravity; }
		P2D_FORCE_INL f32v2 GetGravity() const { return m_Gravity; }

		P2D_FORCE_INL void SetAutoClearForces(bool autoClear) { m_AutoClearForces = autoClear; }

		P2D_FORCE_INL void SetContactFilter(const ContactFilter& filter) { m_ContactFilter = filter; }
		P2D_FORCE_INL void ResetContactFilter() { m_ContactFilter = ContactFilter(); }

		P2D_FORCE_INL EventListener& GetEventListener() { return m_EventListener; }

	private:
		friend class Body;
		friend class Shape;
		friend class BroadPhase;
		friend class ContactManager;

		void IntegrateVelocity(f32 timestep);
		void IntegratePosition(f32 timestep);

		f32 m_Dt;
		f32 m_Timestep;
		bool m_AutoClearForces;

		f32v2 m_Gravity;
		
		Body* m_pBodyList;
		u32 m_BodyCount;
		BlockAllocator m_Allocator;

		EventListener m_EventListener;
		BroadPhase* m_pBroadPhase;
		ContactManager* m_pContactManager;
		ContactFilter m_ContactFilter;
	};

}
