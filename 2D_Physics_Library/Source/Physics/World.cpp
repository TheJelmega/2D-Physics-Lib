#include "Physics/World.h"
#include "Physics/Contacts/ContactSolver.h"
#include "Physics/PhysicsSolver.h"
#include "Physics/BroadPhase.h"

namespace P2D {

	World::World()
		: m_Dt(0)
		, m_Timestep(1.f / 30.f)
		, m_AutoClearForces(true)
		, m_Gravity(0, -9.81)
		, m_pBodyList(nullptr)
		, m_BodyCount(0)
		, m_ShapeCount(0)
		, m_pConstraintList(nullptr)
		, m_ConstraintCount(0)
		, m_pJointList(nullptr)
		, m_JointCount(0)
	{
		m_pPhysicsSolver = new PhysicsSolver(this, &m_StackAllocator);
		m_pBroadPhase = new BroadPhase(this);
		m_pRaycaster = new Raycaster(this);
	}

	World::~World()
	{
		delete m_pRaycaster;
		delete m_pBroadPhase;
		delete m_pPhysicsSolver;
	}

	Body* World::CreateBody(const BodyDef& def)
	{
		Body* pBody = static_cast<Body*>(m_Allocator.Allocate(sizeof(Body)));
		new (pBody) Body(def, this);

		pBody->m_pNext = m_pBodyList;
		if (m_pBodyList)
			m_pBodyList->m_pPrev = pBody;
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
		++m_ShapeCount;
		return pShape;
	}

	void World::DestroyShape(CircleShape* pShape)
	{
		m_pBroadPhase->RemoveShape(pShape);
		pShape->~CircleShape();
		m_Allocator.Deallocate(pShape, sizeof(CircleShape));
		--m_ShapeCount;
	}

	EdgeShape* World::CreateShape(const EdgeShapeDef& def)
	{
		EdgeShape* pShape = static_cast<EdgeShape*>(m_Allocator.Allocate(sizeof(EdgeShape)));
		new (pShape) EdgeShape(def);
		++m_ShapeCount;
		return pShape;
	}

	void World::DestroyShape(EdgeShape* pShape)
	{
		m_pBroadPhase->RemoveShape(pShape);
		pShape->~EdgeShape();
		m_Allocator.Deallocate(pShape, sizeof(EdgeShape));
		--m_ShapeCount;
	}

	ChainShape* World::CreateShape(const ChainShapeDef& def)
	{
		ChainShape* pShape = static_cast<ChainShape*>(m_Allocator.Allocate(sizeof(ChainShape)));
		new (pShape) ChainShape(def, &m_Allocator);
		++m_ShapeCount;
		return pShape;
	}

	void World::DestroyShape(ChainShape* pShape)
	{
		m_pBroadPhase->RemoveShape(pShape);
		pShape->~ChainShape();
		m_Allocator.Deallocate(pShape, sizeof(ChainShape));
		--m_ShapeCount;
	}

	PolygonShape* World::CreateShape(const PolygonShapeDef& def)
	{
		PolygonShape* pShape = static_cast<PolygonShape*>(m_Allocator.Allocate(sizeof(PolygonShape)));
		new (pShape) PolygonShape(def, &m_Allocator);
		++m_ShapeCount;
		return pShape;
	}

	void World::DestroyShape(PolygonShape* pShape)
	{
		m_pBroadPhase->RemoveShape(pShape);
		pShape->~PolygonShape();
		m_Allocator.Deallocate(pShape, sizeof(PolygonShape));
		--m_ShapeCount;
	}

	Constraint* World::CreateConstraint(const ConstraintDef& def)
	{
		Constraint* pConstraint = static_cast<Constraint*>(m_Allocator.Allocate(sizeof(Constraint)));
		new (pConstraint) Constraint(def);

		pConstraint->m_pNext = m_pConstraintList;
		if (m_pConstraintList)
			m_pConstraintList->m_pPrev = pConstraint;
		m_pConstraintList = pConstraint;
		
		++m_ConstraintCount;
		return pConstraint;
	}

	void World::DestroyConstraint(Constraint* pConstraint)
	{
		// Remove from world
		if (pConstraint->m_pNext)
			pConstraint->m_pNext->m_pPrev = pConstraint->m_pPrev;
		if (pConstraint->m_pPrev)
			pConstraint->m_pPrev->m_pNext = pConstraint->m_pNext;
		if (pConstraint == m_pConstraintList)
			m_pConstraintList = pConstraint->m_pNext;


		pConstraint->~Constraint();
		m_Allocator.Deallocate(pConstraint, sizeof(Constraint));
		--m_ConstraintCount;
	}

	RevoluteJoint* World::CreateJoint(const RevoluteJointDef& def)
	{
		P2D_ASSERT(def.pBody1 && def.pBody1->m_Type != BodyType::Static);
		RevoluteJoint* pJoint = static_cast<RevoluteJoint*>(m_Allocator.Allocate(sizeof(RevoluteJoint)));
		new (pJoint) RevoluteJoint(def);

		//Add to world
		pJoint->m_pNext = m_pJointList;
		if (m_pJointList)
			m_pJointList->m_pPrev = pJoint;
		m_pJointList = pJoint;

		//Add to body 0
		if (def.pBody0)
		{
			pJoint->m_Node0.pNext = def.pBody0->m_pJointList;
			if (def.pBody0->m_pJointList)
				def.pBody0->m_pJointList->pPrev = &pJoint->m_Node0;
			def.pBody0->m_pJointList = &pJoint->m_Node0;
		}

		//Add to body 1
		pJoint->m_Node1.pNext = def.pBody1->m_pJointList;
		if (def.pBody1->m_pJointList)
			def.pBody1->m_pJointList->pPrev = &pJoint->m_Node1;
		def.pBody1->m_pJointList = &pJoint->m_Node1;

		++m_JointCount;
		return pJoint;
	}

	void World::DestroyJoint(RevoluteJoint* pJoint)
	{
		//Remove from world
		if (pJoint->m_pNext)
			pJoint->m_pNext->m_pPrev = pJoint->m_pPrev;
		if (pJoint->m_pPrev)
			pJoint->m_pPrev->m_pNext = pJoint->m_pNext;
		if (m_pJointList == pJoint)
			m_pJointList = pJoint->m_pNext;

		//Remove from body 0
		if (pJoint->m_pBody1)
		{
			if (pJoint->m_Node0.pNext)
				pJoint->m_Node0.pNext->pPrev = pJoint->m_Node0.pPrev;
			if (pJoint->m_Node0.pPrev)
				pJoint->m_Node0.pPrev->pNext = pJoint->m_Node0.pNext;
			if (pJoint->m_pBody0->m_pJointList == &pJoint->m_Node0)
				pJoint->m_pBody0->m_pJointList = pJoint->m_Node0.pNext;
		}

		//Remove from body 1
		if (pJoint->m_Node1.pNext)
			pJoint->m_Node1.pNext->pPrev = pJoint->m_Node1.pPrev;
		if (pJoint->m_Node1.pPrev)
			pJoint->m_Node1.pPrev->pNext = pJoint->m_Node1.pNext;
		if (pJoint->m_pBody1->m_pJointList == &pJoint->m_Node1)
			pJoint->m_pBody1->m_pJointList = pJoint->m_Node1.pNext;

		pJoint->~RevoluteJoint();
		m_Allocator.Deallocate(pJoint, sizeof(RevoluteJoint));
		--m_JointCount;
	}

	DistanceJoint* World::CreateJoint(const DistanceJointDef& def)
	{
		P2D_ASSERT(def.pBody1 && def.pBody1->m_Type != BodyType::Static);
		DistanceJoint* pJoint = static_cast<DistanceJoint*>(m_Allocator.Allocate(sizeof(DistanceJoint)));
		new (pJoint) DistanceJoint(def);

		//Add to world
		pJoint->m_pNext = m_pJointList;
		if (m_pJointList)
			m_pJointList->m_pPrev = pJoint;
		m_pJointList = pJoint;

		//Add to body 0
		if (def.pBody0)
		{
			pJoint->m_Node0.pNext = def.pBody0->m_pJointList;
			if (def.pBody0->m_pJointList)
				def.pBody0->m_pJointList->pPrev = &pJoint->m_Node0;
			def.pBody0->m_pJointList = &pJoint->m_Node0;
		}

		//Add to body 1
		pJoint->m_Node1.pNext = def.pBody1->m_pJointList;
		if (def.pBody1->m_pJointList)
			def.pBody1->m_pJointList->pPrev = &pJoint->m_Node1;
		def.pBody1->m_pJointList = &pJoint->m_Node1;

		++m_JointCount;
		return pJoint;
	}

	void World::DestroyJoint(DistanceJoint* pJoint)
	{
		//Remove from world
		if (pJoint->m_pNext)
			pJoint->m_pNext->m_pPrev = pJoint->m_pPrev;
		if (pJoint->m_pPrev)
			pJoint->m_pPrev->m_pNext = pJoint->m_pNext;
		if (m_pJointList == pJoint)
			m_pJointList = pJoint->m_pNext;

		//Remove from body 0
		if (pJoint->m_pBody1)
		{
			if (pJoint->m_Node0.pNext)
				pJoint->m_Node0.pNext->pPrev = pJoint->m_Node0.pPrev;
			if (pJoint->m_Node0.pPrev)
				pJoint->m_Node0.pPrev->pNext = pJoint->m_Node0.pNext;
			if (pJoint->m_pBody0->m_pJointList == &pJoint->m_Node0)
				pJoint->m_pBody0->m_pJointList = pJoint->m_Node0.pNext;
		}

		//Remove from body 1
		if (pJoint->m_Node1.pNext)
			pJoint->m_Node1.pNext->pPrev = pJoint->m_Node1.pPrev;
		if (pJoint->m_Node1.pPrev)
			pJoint->m_Node1.pPrev->pNext = pJoint->m_Node1.pNext;
		if (pJoint->m_pBody1->m_pJointList == &pJoint->m_Node1)
			pJoint->m_pBody1->m_pJointList = pJoint->m_Node1.pNext;

		pJoint->~DistanceJoint();
		m_Allocator.Deallocate(pJoint, sizeof(DistanceJoint));
		--m_JointCount;
	}

	FixedJoint* World::CreateJoint(const FixedJointDef& def)
	{
		P2D_ASSERT(def.pBody1 && def.pBody1->m_Type != BodyType::Static);
		FixedJoint* pJoint = static_cast<FixedJoint*>(m_Allocator.Allocate(sizeof(FixedJoint)));
		new (pJoint) FixedJoint(def);

		//Add to world
		pJoint->m_pNext = m_pJointList;
		if (m_pJointList)
			m_pJointList->m_pPrev = pJoint;
		m_pJointList = pJoint;

		//Add to body 0
		if (def.pBody0)
		{
			pJoint->m_Node0.pNext = def.pBody0->m_pJointList;
			if (def.pBody0->m_pJointList)
				def.pBody0->m_pJointList->pPrev = &pJoint->m_Node0;
			def.pBody0->m_pJointList = &pJoint->m_Node0;
		}

		//Add to body 1
		pJoint->m_Node1.pNext = def.pBody1->m_pJointList;
		if (def.pBody1->m_pJointList)
			def.pBody1->m_pJointList->pPrev = &pJoint->m_Node1;
		def.pBody1->m_pJointList = &pJoint->m_Node1;

		++m_JointCount;
		return pJoint;
	}

	void World::DestroyJoint(FixedJoint* pJoint)
	{
		//Remove from world
		if (pJoint->m_pNext)
			pJoint->m_pNext->m_pPrev = pJoint->m_pPrev;
		if (pJoint->m_pPrev)
			pJoint->m_pPrev->m_pNext = pJoint->m_pNext;
		if (m_pJointList == pJoint)
			m_pJointList = pJoint->m_pNext;

		//Remove from body 0
		if (pJoint->m_pBody1)
		{
			if (pJoint->m_Node0.pNext)
				pJoint->m_Node0.pNext->pPrev = pJoint->m_Node0.pPrev;
			if (pJoint->m_Node0.pPrev)
				pJoint->m_Node0.pPrev->pNext = pJoint->m_Node0.pNext;
			if (pJoint->m_pBody0->m_pJointList == &pJoint->m_Node0)
				pJoint->m_pBody0->m_pJointList = pJoint->m_Node0.pNext;
		}

		//Remove from body 1
		if (pJoint->m_Node1.pNext)
			pJoint->m_Node1.pNext->pPrev = pJoint->m_Node1.pPrev;
		if (pJoint->m_Node1.pPrev)
			pJoint->m_Node1.pPrev->pNext = pJoint->m_Node1.pNext;
		if (pJoint->m_pBody1->m_pJointList == &pJoint->m_Node1)
			pJoint->m_pBody1->m_pJointList = pJoint->m_Node1.pNext;

		pJoint->~FixedJoint();
		m_Allocator.Deallocate(pJoint, sizeof(FixedJoint));
		--m_JointCount;
	}

	PrismaticJoint* World::CreateJoint(const PrismaticJointDef& def)
	{
		P2D_ASSERT(def.pBody1 && def.pBody1->m_Type != BodyType::Static);
		PrismaticJoint* pJoint = static_cast<PrismaticJoint*>(m_Allocator.Allocate(sizeof(PrismaticJoint)));
		new (pJoint) PrismaticJoint(def);

		//Add to world
		pJoint->m_pNext = m_pJointList;
		if (m_pJointList)
			m_pJointList->m_pPrev = pJoint;
		m_pJointList = pJoint;

		//Add to body 0
		if (def.pBody0)
		{
			pJoint->m_Node0.pNext = def.pBody0->m_pJointList;
			if (def.pBody0->m_pJointList)
				def.pBody0->m_pJointList->pPrev = &pJoint->m_Node0;
			def.pBody0->m_pJointList = &pJoint->m_Node0;
		}

		//Add to body 1
		pJoint->m_Node1.pNext = def.pBody1->m_pJointList;
		if (def.pBody1->m_pJointList)
			def.pBody1->m_pJointList->pPrev = &pJoint->m_Node1;
		def.pBody1->m_pJointList = &pJoint->m_Node1;

		++m_JointCount;
		return pJoint;
	}

	void World::DestroyJoint(PrismaticJoint* pJoint)
	{
		//Remove from world
		if (pJoint->m_pNext)
			pJoint->m_pNext->m_pPrev = pJoint->m_pPrev;
		if (pJoint->m_pPrev)
			pJoint->m_pPrev->m_pNext = pJoint->m_pNext;
		if (m_pJointList == pJoint)
			m_pJointList = pJoint->m_pNext;

		//Remove from body 0
		if (pJoint->m_pBody1)
		{
			if (pJoint->m_Node0.pNext)
				pJoint->m_Node0.pNext->pPrev = pJoint->m_Node0.pPrev;
			if (pJoint->m_Node0.pPrev)
				pJoint->m_Node0.pPrev->pNext = pJoint->m_Node0.pNext;
			if (pJoint->m_pBody0->m_pJointList == &pJoint->m_Node0)
				pJoint->m_pBody0->m_pJointList = pJoint->m_Node0.pNext;
		}

		//Remove from body 1
		if (pJoint->m_Node1.pNext)
			pJoint->m_Node1.pNext->pPrev = pJoint->m_Node1.pPrev;
		if (pJoint->m_Node1.pPrev)
			pJoint->m_Node1.pPrev->pNext = pJoint->m_Node1.pNext;
		if (pJoint->m_pBody1->m_pJointList == &pJoint->m_Node1)
			pJoint->m_pBody1->m_pJointList = pJoint->m_Node1.pNext;

		pJoint->~PrismaticJoint();
		m_Allocator.Deallocate(pJoint, sizeof(PrismaticJoint));
		--m_JointCount;
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

		if (m_pBodyList == nullptr)
			return;

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

				// Don't add if the contact is already in the solver
				if (pB->m_InSolver)
					continue;

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

	void World::Raycast(const RaycastInput& input, RaycastOutput& output)
	{
		m_pRaycaster->Raycast(input, output);
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
