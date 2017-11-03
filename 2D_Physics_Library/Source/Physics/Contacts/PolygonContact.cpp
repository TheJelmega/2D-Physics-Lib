#include "Physics/Contacts/PolygonContact.h"
#include "Physics/Collision.h"
#include "Physics/Shapes/Shape.h"
#include "Physics/Body.h"

namespace P2D {


	Contact* PolygonContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		PolygonContact* pContact = static_cast<PolygonContact*>(pAlloc->Allocate(sizeof(PolygonContact)));
		new (pContact) PolygonContact(pShape0, pShape1);
		return pContact;
	}

	void PolygonContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<PolygonContact*>(pContact)->~PolygonContact();
		pAlloc->Deallocate(pContact, sizeof(PolygonContact));
	}

	void PolygonContact::Evaluate(Manifold& manifold)
	{
		Collision::PolygonAndPolygon(reinterpret_cast<PolygonShape*>(m_pShape0), m_pShape0->GetBody()->GetTransform(), reinterpret_cast<PolygonShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), manifold);
	}

	PolygonContact::PolygonContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	PolygonContact::~PolygonContact()
	{
	}
}
