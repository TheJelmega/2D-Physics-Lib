#include "Physics/World.h"
#include <iostream>
#include "Physics/Contacts/ContactSolver.h"

namespace P2D {

	World::World()
		: m_Dt(0)
		, m_Timestep(1.f / 30.f)
		, m_AutoClearForces(true)
		, m_Gravity(0, -9.81)
		, m_pBodyList(nullptr)
		, m_BodyCount(0)
	{
		m_pBroadPhase = new BroadPhase(this);
		m_pContactManager = new ContactManager(this);
	}

	World::~World()
	{
		delete m_pBroadPhase;
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
		//Prepare solver (disgarding inactive objects, static objects, ...)

		IntegrateVelocity(timestep);

		m_pBroadPhase->UpdatePairs(m_pContactManager);
		m_pContactManager->UpdateCollisions();

		ContactSolver solver;
		solver.SolveVelocity(timestep);

		IntegratePosition(timestep);
		solver.SolvePosition(timestep);
	}

	void World::ClearBodyForces()
	{
		for (Body* pBody = m_pBodyList; pBody; pBody = pBody->m_pNext)
		{
			pBody->m_Force = f32v2::Zero;
			pBody->m_Torque = 0.f;
		}
	}

	void World::IntegrateVelocity(f32 timestep)
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
	}

}
