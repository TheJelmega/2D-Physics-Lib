#include "Physics/Contacts/CircleContact.h"
#include "Physics/Collision.h"
#include "Physics/Body.h"

namespace P2D {


	Contact* CircleContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		CircleContact* pContact = static_cast<CircleContact*>(pAlloc->Allocate(sizeof(CircleContact)));
		new (pContact) CircleContact(pShape0, pShape1);
		return pContact;
	}

	void CircleContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<CircleContact*>(pContact)->~CircleContact();
		pAlloc->Deallocate(pContact, sizeof(CircleContact));
	}

	void CircleContact::Evaluate(Manifold& manifold)
	{
		Collision::CircleAndCircle(static_cast<CircleShape*>(m_pShape0), m_pShape0->GetBody()->GetTransform(), static_cast<CircleShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), manifold);
	}

	CircleContact::CircleContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	CircleContact::~CircleContact()
	{
	}
}
