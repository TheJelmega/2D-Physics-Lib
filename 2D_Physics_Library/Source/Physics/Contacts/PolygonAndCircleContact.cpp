#include "Physics/Contacts/PolygonAndCircleContact.h"
#include "Physics/Collision.h"
#include "Physics/Shapes/Shape.h"
#include "Physics/Body.h"

namespace P2D {


	Contact* PolygonAndCircleContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		PolygonAndCircleContact* pContact = static_cast<PolygonAndCircleContact*>(pAlloc->Allocate(sizeof(PolygonAndCircleContact)));
		new (pContact) PolygonAndCircleContact(pShape0, pShape1);
		return pContact;
	}

	void PolygonAndCircleContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<PolygonAndCircleContact*>(pContact)->~PolygonAndCircleContact();
		pAlloc->Deallocate(pContact, sizeof(PolygonAndCircleContact));
	}

	void PolygonAndCircleContact::Evaluate(Manifold& manifold)
	{
		Collision::PolgonAndCircle(reinterpret_cast<PolygonShape*>(m_pShape0), m_pShape0->GetBody()->GetTransform(), reinterpret_cast<CircleShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), manifold);
	}

	PolygonAndCircleContact::PolygonAndCircleContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	PolygonAndCircleContact::~PolygonAndCircleContact()
	{
	}
}
