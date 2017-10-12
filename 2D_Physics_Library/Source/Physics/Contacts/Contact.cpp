#include "Physics/Contacts/Contact.h"
#include "Physics/Contacts/CircleContact.h"
#include "Physics/Body.h"
#include "Physics/World.h"
#include "Physics/EventListener.h"

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
		Shape::Type type1 = pShape0->GetType();

		ContactRegister::ContactCreateFunc createFunc = m_ContactRegisters[i32(type0)][i32(type1)].createFunc;
		if (createFunc)
		{
			if (m_ContactRegisters[i32(type0)][i32(type1)].primary)
				return createFunc(pShape0, pShape1, pAlloc);
			return createFunc(pShape1, pShape0, pAlloc);
		}
		return nullptr;
	}

	void Contact::Destroy(Contact* contact, BlockAllocator* pAlloc)
	{
		m_ContactRegisters[i32(contact->m_pShape0->GetType())][i32(contact->m_pShape1->GetType())].destroyFunc(contact, pAlloc);
	}

	void Contact::Evaluate(Manifold& manifold)
	{
		manifold.numPoints = 0;
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

		m_Initialized = true;
	}

	Contact::Contact()
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pShape0(nullptr)
		, m_pShape1(nullptr)
		, m_Touching(false)
		, m_CheckFilter(false)
	{
	}

	Contact::Contact(Shape* pShape0, Shape* pShape1)
		: m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_pShape0(pShape0)
		, m_pShape1(pShape1)
		, m_Touching(false)
		, m_CheckFilter(false)
	{
	}

	void Contact::Update()
	{
		World* pWorld = m_pShape0->GetBody()->GetWorld();

		bool sensor = m_pShape0->IsSensor() || m_pShape1->IsSensor();

		bool touching = false;
		bool wasTouching = m_Touching;

		EventListener& eventListener = pWorld->GetEventListener();

		if (sensor)
		{
			//TODO Check if overlap
		}
		else
		{
			Manifold manifold;
			Evaluate(manifold);

			touching = m_Touching = manifold.numPoints > 0;
		}

		if (touching && !wasTouching)
			eventListener.OnCollisionEnter(this);
		else if (!touching && wasTouching)
			eventListener.OnCollisionLeave(this);
		else if (touching && wasTouching)
			eventListener.OnCollisionStay(this);
	}
}
