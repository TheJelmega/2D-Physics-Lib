#include "Physics/World.h"
#include <iostream>
#include "Physics/Contacts/ContactSolver.h"
#include "Physics/PhysicsSolver.h"

namespace P2D {

	World::World()
		: m_Dt(0)
		, m_Timestep(1.f / 30.f)
		, m_AutoClearForces(true)
		, m_Gravity(0, -9.81)
		, m_pBodyList(nullptr)
		, m_BodyCount(0)
	{
		m_pPhysicsSolver = new PhysicsSolver(this, &m_StackAllocator);
	}

	World::~World()
	{
		delete m_pPhysicsSolver;
	}

	Body* World::CreateBody(const BodyDef& def)
	{
		Body* pBody = static_cast<Body*>(m_Allocator.Allocate(sizeof(Body)));
		new (pBody) Body(def, this);

		if (m_pBodyList)
		{
			m_pBodyList->m_pPrev = pBody;
			pBody->m_pNext = m_pBodyList;
		}
		m_pBodyList = pBody;


		++m_BodyCount;
		return pBody;
	}

	void World::DestroyBody(Body* pBody)
	{
		//TODO: Delete joint / constraints

		//Delete shapes
		for (Shape* pShape = pBody->GetShapes(); pShape; pShape = pShape->GetNext())
		{
			switch (pShape->GetType())
			{
			case Shape::Type::None: break;
			case Shape::Type::Circle:
				DestroyShape(static_cast<CircleShape*>(pShape));
				break;
			case Shape::Type::Polygon: break;
			default: 
				break;
			}
		}

		//Remove body from body list
		if (pBody->m_pNext)
			pBody->m_pNext->m_pPrev = pBody->m_pPrev;
		if (pBody->m_pPrev)
			pBody->m_pPrev->m_pNext = pBody->m_pNext;
		if (m_pBodyList == pBody)
			m_pBodyList = pBody->m_pNext;

		//Delete body
		pBody->~Body();
		m_Allocator.Deallocate(pBody, sizeof(pBody));
		--m_BodyCount;
	}

	CircleShape* World::CreateShape(const CircleShapeDef& def)
	{
		CircleShape* pShape = static_cast<CircleShape*>(m_Allocator.Allocate(sizeof(CircleShape)));
		new (pShape) CircleShape(def);
		return pShape;
	}

	void World::DestroyShape(CircleShape* pShape)
	{
		pShape->~CircleShape();
		m_Allocator.Deallocate(pShape, sizeof(CircleShape));
	}

	EdgeShape* World::CreateShape(const EdgeShapeDef& def)
	{
		EdgeShape* pShape = static_cast<EdgeShape*>(m_Allocator.Allocate(sizeof(EdgeShape)));
		new (pShape) EdgeShape(def);
		return pShape;
	}

	void World::DestroyShape(EdgeShape* pShape)
	{
		pShape->~EdgeShape();
		m_Allocator.Deallocate(pShape, sizeof(EdgeShape));
	}

	ChainShape* World::CreateShape(const ChainShapeDef& def)
	{
		ChainShape* pShape = static_cast<ChainShape*>(m_Allocator.Allocate(sizeof(ChainShape)));
		new (pShape) ChainShape(def, &m_Allocator);
		return pShape;
	}

	void World::DestroyShape(ChainShape* pShape)
	{
		pShape->Dealloc(&m_Allocator);
		pShape->~ChainShape();
		m_Allocator.Deallocate(pShape, sizeof(ChainShape));
	}

	void World::Update(f32 dt)
	{
		dt = Math::Min(dt, g_MaxDeltaTime);
		m_Dt += dt;
		for (; m_Dt >= m_Timestep; m_Dt -= m_Timestep)
		{
			Step(m_Timestep);
		}

		if (m_AutoClearForces)
			ClearBodyForces();
	}

	void World::Step(f32 timestep)
	{
		//Update collisions
		//m_pPhysicsSolver->UpdateCollisions();

		// Clear solver
		m_pPhysicsSolver->Clear();

		// Clear solver flag
		for (Body* pBody = m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			pBody->m_InSolver = false;
		}
		for (Contact* pContact = m_pPhysicsSolver->m_pContactManager->m_pContactList; pContact; pContact = pContact->m_pNext)
		{
			pContact->m_InSolver = false;
			pContact->m_pNextTouching = nullptr;
		}

		// Prepare solver (disgarding inactive objects, static objects, ...)
		PhysicsSolverSettings settings;
		settings.numBodies = m_BodyCount;

		m_pPhysicsSolver->Prepare(settings);

		Body** stack = static_cast<Body**>(m_StackAllocator.Allocate(m_BodyCount * sizeof(Body*)));
		
		for (Body* pBody = m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			// Don't add if the contact is already in the solver
			if (pBody->m_InSolver)
				continue;
			// Don't add if the contact is static, not active or sleeping
			if (pBody->m_Type == BodyType::Static || !pBody->m_Active || !pBody->m_Awake)
				continue;

			u32 stackCount = 0;
			stack[stackCount++] = pBody;

			while (stackCount > 0)
			{

				Body* pB = stack[--stackCount];
				m_pPhysicsSolver->Add(pB);

				// If the body is static, don't bother to search
				if (pB->m_Type == BodyType::Static)
					continue;

				for (ContactNode* pNode = pB->m_pContactList; pNode; pNode = pNode->pNext)
				{
					Contact* pContact = pNode->pContact;
					// Don't add if the contact is already in the solver
					if (pNode->pContact->m_InSolver)
						continue;

					// Don't add if contact isn't touching
					if (!pNode->pContact->m_Touching)
						continue;

					// Don't add if 1 shape is a sensor
					bool sensor0 = pContact->m_pShape0->m_Sensor;
					bool sensor1 = pContact->m_pShape1->m_Sensor;
					if (sensor0 || sensor1)
						continue;

					m_pPhysicsSolver->Add(pNode->pContact);

					// If the other body isn't added, add it to the stack
					if (!pNode->pOther->m_InSolver)
						stack[stackCount++] = pNode->pOther;
				}
			}
		}
		m_StackAllocator.Deallocate(stack);

		// Solve
		m_pPhysicsSolver->Solve(timestep);

		/*IntegrateVelocity(timestep);

		m_pBroadPhase->UpdatePairs(m_pContactManager);
		m_pContactManager->UpdateCollisions();

		ContactSolver solver;
		solver.SolveVelocity(timestep);

		IntegratePosition(timestep);
		solver.SolvePosition(timestep);*/
	}

	void World::ClearBodyForces()
	{
		for (Body* pBody = m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			pBody->m_Force = f32v2::Zero;
			pBody->m_Torque = 0.f;
		}
	}

	/*void World::IntegrateVelocity(f32 timestep)
	{
		for (Body* pBody = m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			if (!pBody->m_Active || !pBody->m_Awake || pBody->m_Type == BodyType::Static)
				continue;

			// linear velocity
			pBody->m_LinearVelocity += m_Gravity * timestep;
			pBody->m_LinearVelocity += pBody->m_Force / pBody->m_MassData.mass * timestep;

			// angular velocity
			pBody->m_AngularVelocity += pBody->m_Torque / pBody->m_MassData.inertia * timestep;
		}
	}

	void World::IntegratePosition(f32 timestep)
	{
		for (Body* pBody = m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			if (!pBody->m_Active || !pBody->m_Awake || pBody->m_Type == BodyType::Static)
				continue;

			pBody->m_Position += pBody->m_LinearVelocity * timestep;
			pBody->m_Angle += pBody->m_AngularVelocity * timestep;
			pBody->UpdateAABB();
		}
	}*/

}
