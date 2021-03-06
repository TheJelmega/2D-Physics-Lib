#include "Physics/PhysicsSolver.h"
#include "Physics/BroadPhase.h"
#include "Physics/Contacts/ContactManager.h"
#include "Physics/Body.h"
#include "Physics/World.h"
#include "Common/StackAllocator.h"
#include "Physics/Contacts/ContactSolver.h"
#include "Physics/Constraint.h"

namespace P2D {


	PhysicsSolverSettings::PhysicsSolverSettings()
		: numBodies(0)
	{
	}

	PhysicsSolver::PhysicsSolver(World* pWorld, StackAllocator* pAllocator)
		: m_pWorld(pWorld)
		, m_pAllocator(pAllocator)
		, m_BodyCount(0)
		, m_BodyCapacity(0)
		, m_ContactCount(0)
		, m_pBodies(nullptr)
		, m_Transforms(nullptr)
		, m_Velocities(nullptr)
		, m_pTouchingContact(nullptr)
		, m_ResitutionCombinationType(ResitutionCombinationType::Min)
		, m_StaticFrictionCombinationType(FrictionCombinationType::Min)
		, m_DynamicFrictionCombinationType(FrictionCombinationType::Min)
	{
		m_pContactManager = new ContactManager(m_pWorld);
	}

	PhysicsSolver::~PhysicsSolver()
	{
		Clear();
		delete m_pContactManager;
	}

	void PhysicsSolver::Prepare(const PhysicsSolverSettings& settings)
	{
		if (settings.numBodies == 0)
			return;

		m_BodyCapacity = settings.numBodies;
		m_pBodies = static_cast<Body**>(m_pAllocator->Allocate(m_BodyCapacity * sizeof(Body*)));
		m_Transforms = static_cast<Transform*>(m_pAllocator->Allocate(m_BodyCapacity * sizeof(Transform)));
		m_Velocities = static_cast<Velocity*>(m_pAllocator->Allocate(m_BodyCapacity * sizeof(Velocity)));
	}

	void PhysicsSolver::Add(Body* pBody)
	{
		P2D_ASSERT(m_BodyCount < m_BodyCapacity);
		pBody->m_SolverIndex = m_BodyCount;
		m_pBodies[m_BodyCount] = pBody;m_Transforms[m_BodyCount] = pBody->m_Transform;
		m_Velocities[m_BodyCount] = pBody->m_Velocity;
		pBody->m_InSolver = true;
		++m_BodyCount;
	}

	void PhysicsSolver::Add(Contact* pContact)
	{
		pContact->m_pNextTouching = m_pTouchingContact;
		m_pTouchingContact = pContact;
		pContact->m_InSolver = true;
		++m_ContactCount;
	}

	/*void PhysicsSolver::UpdateCollisions()
	{
		//Update broad phase
		m_pBroadPhase->UpdatePairs(m_pContactManager);

		//Update narrow phase
		m_pContactManager->UpdateCollisions();
	}*/

	void PhysicsSolver::Solve(f32 dt)
	{

		//Integrate Velocities
		for (u32 i = 0; i < m_BodyCount; ++i)
		{
			Body* pBody = m_pBodies[i];
			if (pBody->m_Type != BodyType::Dynamic)
				continue;

			const BodyMassData& massData = pBody->m_MassData;
			Velocity& vel = m_Velocities[i];

			vel.linearVelocity += m_pWorld->m_Gravity * dt;
			vel.linearVelocity += pBody->m_Force * massData.invMass * dt;
			vel.angularVelocity += pBody->m_Torque * massData.invInertia * dt;
		}

		//Integrate position
		for (u32 i = 0; i < m_BodyCount; ++i)
		{
			if (m_pBodies[i]->m_Type == BodyType::Static)
				continue;
			Transform& transform = m_Transforms[i];
			//transform.position += m_Velocities[i].linearVelocity * dt;
			f32 angle = m_Velocities[i].angularVelocity * dt;
			f32v2 comLoc = transform.Move(m_pBodies[i]->m_MassData.centerOfMass);
			transform.position.RotateAroundPoint(comLoc, angle);
			transform.position += m_Velocities[i].linearVelocity * dt;
			transform.rotation += angle;
		}

		//Update bodies for collision detection
		for (u32 i = 0; i < m_BodyCount; ++i)
		{
			m_pBodies[i]->m_Transform = m_Transforms[i];
			m_pBodies[i]->m_Velocity = m_Velocities[i];
		}

		//Update constrains
		for (Constraint* pConstraint = m_pWorld->m_pConstraintList; pConstraint; pConstraint = pConstraint->m_pNext)
		{
			pConstraint->Update();
		}

		//Solve joints
		for (Joint* pJoint = m_pWorld->m_pJointList; pJoint; pJoint = pJoint->m_pNext)
		{
			pJoint->Update(dt);
		}

		//Update AABBs
		for (u32 i = 0; i < m_BodyCount; ++i)
		{
			for (Shape* pShape = m_pBodies[i]->m_pShape; pShape; pShape = pShape->m_pNext)
				m_pWorld->m_pBroadPhase->UpdateShape(pShape);
			m_pBodies[i]->UpdateAABB();
			m_Transforms[i] = m_pBodies[i]->m_Transform;
			m_Velocities[i] = m_pBodies[i]->m_Velocity;
		}

		//Update collisions
		m_pWorld->m_pBroadPhase->UpdatePairs(m_pContactManager);
		m_pContactManager->UpdateCollisions();

		//Add new contacts and awoken bodies
		AddNewBodies();

		//Create ContactSolver
		ContactSolver contactSolver(this);

		//Solve velocity
		contactSolver.SolveVelocity(dt);

		//Solve position
		contactSolver.SolvePosition(dt);

		//Update bodies after solving
		for (u32 i = 0; i < m_BodyCount; ++i)
		{
			m_pBodies[i]->m_Transform = m_Transforms[i];
			m_pBodies[i]->m_Velocity = m_Velocities[i];
		}

		//Solve joints
		for (Joint* pJoint = m_pWorld->m_pJointList; pJoint; pJoint = pJoint->m_pNext)
		{
			pJoint->Update(dt);
		}

		//Update AABB, limit linear and angular velocity and update sleep
		for (u32 i = 0; i < m_BodyCount; ++i)
		{
			Body* pBody = m_pBodies[i];

			for (Shape* pShape = m_pBodies[i]->m_pShape; pShape; pShape = pShape->m_pNext)
				m_pWorld->m_pBroadPhase->UpdateShape(pShape);
			pBody->UpdateAABB();

			//linear
			constexpr f32 maxLinVel = g_MaxLinearVelocity * g_MaxLinearVelocity;
			f32 l = pBody->m_LinearVelocity.SqLength();
			if (l > maxLinVel)
			{
				f32 l0 = sqrt(l);
				pBody->m_LinearVelocity *= (g_MaxLinearVelocity / l0);
			}

			//angular
			if (pBody->m_AngularVelocity < -g_MaxAngulareVelocity)
				pBody->m_AngularVelocity = -g_MaxAngulareVelocity;
			if (pBody->m_AngularVelocity > g_MaxAngulareVelocity)
				pBody->m_AngularVelocity = g_MaxAngulareVelocity;

			constexpr f32 maxSleepVel = g_MaxSleepLinearVelocity * g_MaxSleepLinearVelocity;
			if (l < maxSleepVel && abs(pBody->m_AngularVelocity) < g_MaxSleepAngularVelocity)
			{
				pBody->m_SleepTimer += dt;
				if (pBody->m_SleepTimer >= g_TimeToSleep)
					pBody->SetAwake(false);
			}
			else
				pBody->m_SleepTimer = 0.f;
		}
	}

	void PhysicsSolver::Clear()
	{
		if (m_Velocities)
			m_pAllocator->Deallocate(m_Velocities);
		if (m_Transforms)
			m_pAllocator->Deallocate(m_Transforms);
		if (m_pBodies)
			m_pAllocator->Deallocate(m_pBodies);
		m_BodyCapacity = 0;
		m_BodyCount = 0;
		m_ContactCount = 0;

		m_pTouchingContact = nullptr;
	}

	f32 PhysicsSolver::CombineRestitution(f32 r0, f32 r1) const
	{
		switch (m_ResitutionCombinationType)
		{
		case ResitutionCombinationType::Min:
			return Math::Min(r0, r1);
		case ResitutionCombinationType::Max:
			return Math::Max(r0, r1);
		case ResitutionCombinationType::Average:
			return (r0 + r1) * .5f;
		case ResitutionCombinationType::Multiply:
			return r0 * r1;
		default: 
			return 0.f;
		}
	}

	f32 PhysicsSolver::CombineStaticFriction(f32 f0, f32 f1) const
	{
		switch (m_StaticFrictionCombinationType)
		{
		case FrictionCombinationType::Min:
			return Math::Min(f0, f1);
		case FrictionCombinationType::Max:
			return Math::Max(f0, f1);
		case FrictionCombinationType::Average:
			return (f0 + f1) * .5f;
		case FrictionCombinationType::Multiply:
			return f0 * f1;
		default:
			return 0.f;
		}
	}

	f32 PhysicsSolver::CombineDynamicFriction(f32 f0, f32 f1) const
	{
		switch (m_DynamicFrictionCombinationType)
		{
		case FrictionCombinationType::Min:
			return Math::Min(f0, f1);
		case FrictionCombinationType::Max:
			return Math::Max(f0, f1);
		case FrictionCombinationType::Average:
			return (f0 + f1) * .5f;
		case FrictionCombinationType::Multiply:
			return f0 * f1;
		default:
			return 0.f;
		}
	}

	void PhysicsSolver::AddNewBodies()
	{
		for (Contact* pContact = m_pContactManager->m_pContactList; pContact; pContact = pContact->m_pNext)
		{
			if (pContact->m_InSolver || !pContact->m_Touching || !pContact->m_Active)
				continue;

			Add(pContact);

			Body* pBody = pContact->m_pShape0->GetBody();
			if (!pBody->m_InSolver)
				Add(pBody);

			pBody = pContact->m_pShape1->GetBody();
			if (!pBody->m_InSolver)
				Add(pBody);
		}
	}
}
