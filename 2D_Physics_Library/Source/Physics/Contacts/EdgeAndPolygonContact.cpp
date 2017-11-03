#include "Physics/Contacts/EdgeAndPolygonContact.h"
#include "Physics/Collision.h"
#include "Physics/Shapes/Shape.h"
#include "Physics/Body.h"

namespace P2D {


	Contact* EdgeAndPolygonContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		EdgeAndPolygonContact* pContact = static_cast<EdgeAndPolygonContact*>(pAlloc->Allocate(sizeof(EdgeAndPolygonContact)));
		new (pContact) EdgeAndPolygonContact(pShape0, pShape1);
		return pContact;
	}

	void EdgeAndPolygonContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<EdgeAndPolygonContact*>(pContact)->~EdgeAndPolygonContact();
		pAlloc->Deallocate(pContact, sizeof(EdgeAndPolygonContact));
	}

	void EdgeAndPolygonContact::Evaluate(Manifold& manifold)
	{
		Collision::EdgeAndPolygon(reinterpret_cast<EdgeShape*>(m_pShape0), m_pShape0->GetBody()->GetTransform(), reinterpret_cast<PolygonShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), manifold);
	}

	EdgeAndPolygonContact::EdgeAndPolygonContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	EdgeAndPolygonContact::~EdgeAndPolygonContact()
	{
	}
}
