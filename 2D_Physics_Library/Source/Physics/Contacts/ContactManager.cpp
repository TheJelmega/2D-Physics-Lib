#include "Physics/Contacts/ContactManager.h"
#include "Physics/World.h"

namespace P2D {


	ContactManager::ContactManager(World* pWorld)
		: m_pContactList(nullptr)
		, m_ContactCount(0)
		, m_pWorld(pWorld)
	{
	}

	ContactManager::~ContactManager()
	{
	}

	void ContactManager::AddPair(Shape* pShape0, Shape* pShape1)
	{
		Body* pBody0 = pShape0->m_pBody;
		Body* pBody1 = pShape1->m_pBody;

		if (pBody0 == pBody1)
			return;

		// Check of contact exists, if 1 body has the contact, both have
		for (ContactNode* pNode = pBody0->m_pContactList; pNode; pNode = pNode->pNext)
		{
			if (pNode->pOther != pBody1)
				continue;

			Shape* pS0 = pNode->pContact->m_pShape0;
			Shape* pS1 = pNode->pContact->m_pShape1;

			if (pS0 == pShape0 && pS1 == pShape1)
				return;
			if (pS0 == pShape1 && pS1 == pShape0)
				return;
		}

		//Check joints
		for (JointNode* pNode = pShape0->m_pBody->m_pJointList; pNode; pNode = pNode->pNext)
		{
			if (pNode->pOther == pShape1->m_pBody && !pNode->pJoint->DoShapesCollide())
			{
				return;
			}
		}

		if (!m_pWorld->m_ContactFilter.ShouldCollide(pShape0, pShape1))
			return;

		Contact* pContact = Contact::Create(pShape0, pShape1, &m_pWorld->m_Allocator);
		if (!pContact) // Unsupported contact
			return;
		
		//Update shape order
		pShape0 = pContact->m_pShape0;
		pShape1 = pContact->m_pShape1;
		pBody0 = pShape0->m_pBody;
		pBody1 = pShape1->m_pBody;

		// Add to world
		pContact->m_pNext = m_pContactList;
		if (m_pContactList)
			m_pContactList->m_pPrev = pContact;
		m_pContactList = pContact;

		//Add to body 0
		pContact->m_Node0.pContact = pContact;
		pContact->m_Node0.pOther = pBody1;
		pContact->m_Node0.pNext = pBody0->m_pContactList;
		if (pBody0->m_pContactList)
			pBody0->m_pContactList->pPrev = &pContact->m_Node0;
		pBody0->m_pContactList = &pContact->m_Node0;

		//Add to body 1
		pContact->m_Node1.pContact = pContact;
		pContact->m_Node1.pOther = pBody0;
		pContact->m_Node1.pNext = pBody1->m_pContactList;
		if (pBody1->m_pContactList)
			pBody1->m_pContactList->pPrev = &pContact->m_Node1;
		pBody1->m_pContactList = &pContact->m_Node1;

		if (!pShape0->m_Sensor && !pShape1->m_Sensor)
		{
			pBody0->SetAwake(true);
			pBody1->SetAwake(true);
		}

		m_pWorld->m_EventListener.OnContactCreate(pContact);
		++m_ContactCount;
	}

	void ContactManager::Destroy(Contact* pContact)
	{
		if (pContact->m_Touching)
			m_pWorld->m_EventListener.OnCollisionLeave(pContact);
		m_pWorld->m_EventListener.OnContactDestroy(pContact);

		// Remove from world
		if (pContact->m_pNext)
			pContact->m_pNext->m_pPrev = pContact->m_pPrev;
		if (pContact->m_pPrev)
			pContact->m_pPrev->m_pNext = pContact->m_pNext;
		if (m_pContactList == pContact)
			m_pContactList = pContact->m_pNext;

		//Remove from body 0
		Body* pBody0 = pContact->m_pShape0->m_pBody;
		if (pContact->m_Node0.pNext)
			pContact->m_Node0.pNext->pPrev = pContact->m_Node0.pPrev;
		if (pContact->m_Node0.pPrev)
			pContact->m_Node0.pPrev->pNext = pContact->m_Node0.pNext;
		if (pBody0->m_pContactList == &pContact->m_Node0)
			pBody0->m_pContactList = pContact->m_Node0.pNext;

		//Remove from body 0
		Body* pBody1 = pContact->m_pShape1->m_pBody;
		if (pContact->m_Node1.pNext)
			pContact->m_Node1.pNext->pPrev = pContact->m_Node1.pPrev;
		if (pContact->m_Node1.pPrev)
			pContact->m_Node1.pPrev->pNext = pContact->m_Node1.pNext;
		if (pBody1->m_pContactList == &pContact->m_Node1)
			pBody1->m_pContactList = pContact->m_Node1.pNext;

		Contact::Destroy(pContact, &m_pWorld->m_Allocator);
		--m_ContactCount;
	}

	void ContactManager::UpdateCollisions()
	{
		Contact* pContact = m_pContactList;
		while (pContact)
		{
			Shape* pShape0 = pContact->m_pShape0;
			Shape* pShape1 = pContact->m_pShape1;

			if (pContact->m_CheckFilter)
			{
				if (!m_pWorld->m_ContactFilter.ShouldCollide(pShape0, pShape1))
				{
					Contact* pNuke = pContact;
					pContact = pContact->m_pNext;
					Destroy(pNuke);
					continue;
				}
				pContact->m_CheckFilter = false;
			}

			Body* pBody0 = pShape0->m_pBody;
			Body* pBody1 = pShape1->m_pBody;

			bool active0 = pBody0->m_Active && pBody0->m_Type != BodyType::Static;
			bool active1 = pBody1->m_Active && pBody1->m_Type != BodyType::Static;
			//Check if awake and not falling asleep
			bool awake0 = active0 && pBody0->m_Awake && pBody0->m_SleepTimer <= Math::Epsilon<f32>;
			bool awake1 = active1 && pBody1->m_Awake && pBody1->m_SleepTimer <= Math::Epsilon<f32>;

			if (awake0 || awake1)
			{
				// Make sure both bodies are awake
				pBody0->SetAwake(true);
				pBody1->SetAwake(true);
				pContact->m_Active = true;
			}
			else
				pContact->m_Active = false;

			if (!active0 && !active1)
			{
				pContact->m_Active = false;
				pContact = pContact->m_pNext;
				continue;
			}

			if (!pShape0->m_AABB.Overlaps(pShape1->m_AABB))
			{
				Contact* pNuke = pContact;
				pContact = pContact->m_pNext;
				Destroy(pNuke);
				continue;
			}

			pContact->Update();

			pContact = pContact->m_pNext;
		}
	}
}
