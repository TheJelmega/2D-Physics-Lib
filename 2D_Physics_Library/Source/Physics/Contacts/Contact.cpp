#include "Physics/Contacts/Contact.h"
#include "Physics/Contacts/CircleContact.h"
#include "Physics/Contacts/EdgeAndCircleContact.h"
#include "Physics/Contacts/ChainAndCircleContact.h"
#include "Physics/Contacts/PolygonAndCircleContact.h"
#include "Physics/Contacts/EdgeAndPolygonContact.h"
#include "Physics/Contacts/PolygonContact.h"
#include "Physics/Body.h"
#include "Physics/World.h"
#include "Physics/EventListener.h"
#include "Physics/Contacts/ChainAndPolygonContact.h"

namespace P2D {

	bool Contact::m_Initialized = false;
	ContactRegister Contact::m_ContactRegisters[i32(Shape::Type::Count)][i32(Shape::Type::Count)] = {};

	ContactRegister::ContactRegister()
		: createFunc(nullptr)
		, destroyFunc(nullptr)
		, primary(false)
	{
	}

	ContactNode::ContactNode()
		: pContact(nullptr)
		, pOther(nullptr)
		, pNext(nullptr)
		, pPrev(nullptr)
	{
	}

	Contact* Contact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		if (!m_Initialized)
			InitializeRegisters();

		Shape::Type type0 = pShape0->GetType();
		Shape::Type type1 = pShape1->GetType();

		ContactRegister::ContactCreateFunc createFunc = m_ContactRegisters[i32(type0)][i32(type1)].createFunc;
		if (createFunc)
		{
			if (m_ContactRegisters[i32(type0)][i32(type1)].primary)
				return createFunc(pShape0, pShape1, pAlloc);
			return createFunc(pShape1, pShape0, pAlloc);
		}
		return nullptr;
	}

	void Contact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		m_ContactRegisters[i32(pContact->m_pShape0->GetType())][i32(pContact->m_pShape1->GetType())].destroyFunc(pContact, pAlloc);
	}

	void Contact::Evaluate(Manifold& manifold)
	{
		manifold.numPairs = 0;
	}

	void Contact::RegisterPair(Shape::Type type0, Shape::Type type1, ContactRegister::ContactCreateFunc createFunc, ContactRegister::ContactDestroyFunc destroyFunc)
	{
		m_ContactRegisters[i32(type0)][i32(type1)].createFunc = createFunc;
		m_ContactRegisters[i32(type0)][i32(type1)].destroyFunc = destroyFunc;
		m_ContactRegisters[i32(type0)][i32(type1)].primary = true;

		if (type0 != type1)
		{
			m_ContactRegisters[i32(type1)][i32(type0)].createFunc = createFunc;
			m_ContactRegisters[i32(type1)][i32(type0)].destroyFunc = destroyFunc;
			m_ContactRegisters[i32(type1)][i32(type0)].primary = false;
		}
	}

	void Contact::InitializeRegisters()
	{
		RegisterPair(Shape::Type::Circle, Shape::Type::Circle, CircleContact::Create, CircleContact::Destroy);
		RegisterPair(Shape::Type::Edge, Shape::Type::Circle, EdgeAndCircleContact::Create, EdgeAndCircleContact::Destroy);
		RegisterPair(Shape::Type::Chain, Shape::Type::Circle, ChainAndCircleContact::Create, ChainAndCircleContact::Destroy);
		RegisterPair(Shape::Type::Polygon, Shape::Type::Circle, PolygonAndCircleContact::Create, PolygonAndCircleContact::Destroy);
		RegisterPair(Shape::Type::Edge, Shape::Type::Polygon, EdgeAndPolygonContact::Create, EdgeAndPolygonContact::Destroy);
		RegisterPair(Shape::Type::Chain, Shape::Type::Polygon, ChainAndPolygonContact::Create, ChainAndPolygonContact::Destroy);
		RegisterPair(Shape::Type::Polygon, Shape::Type::Polygon, PolygonContact::Create, PolygonContact::Destroy);

		m_Initialized = true;
	}

	Contact::Contact()
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pNextTouching(nullptr)
		, m_pShape0(nullptr)
		, m_pShape1(nullptr)
		, m_Index0(0)
		, m_Index1(0)
		, m_Touching(false)
		, m_CheckFilter(false)
		, m_InSolver(false)
		, m_Active(true)
	{
	}

	Contact::Contact(Shape* pShape0, Shape* pShape1)
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pNextTouching(nullptr)
		, m_pShape0(pShape0)
		, m_pShape1(pShape1)
		, m_Index0(-1)
		, m_Index1(-1)
		, m_Touching(false)
		, m_CheckFilter(false)
		, m_InSolver(false)
		, m_Active(true)
	{
	}

	void Contact::Update()
	{
		World* pWorld = m_pShape0->GetBody()->GetWorld();

		bool sensor = m_pShape0->IsSensor() || m_pShape1->IsSensor();

		bool wasTouching = m_Touching;
		m_Touching = false;

		EventListener& eventListener = pWorld->GetEventListener();

		if (sensor)
		{
			Evaluate(m_Manifold);

			m_Touching = m_Manifold.numPairs > 0;
			m_Manifold.numPairs = 0;
		}
		else
		{
			Evaluate(m_Manifold);

			m_Touching = m_Manifold.numPairs > 0;

			if (m_Touching)
			{
				m_Index0 = m_pShape0->GetBody()->GetSolverIndex();
				m_Index1 = m_pShape1->GetBody()->GetSolverIndex();
			}
		}

		if (m_Touching && !wasTouching)
			eventListener.OnCollisionEnter(this);
		else if (!m_Touching && wasTouching)
			eventListener.OnCollisionLeave(this);
		else if (m_Touching && wasTouching)
			eventListener.OnCollisionStay(this);
	}
}
