#include "Physics/Contacts/EdgeAndCircleContact.h"
#include "Physics/Collision.h"
#include "Physics/Shapes/Shape.h"
#include "Physics/Body.h"

namespace P2D {

	Contact* EdgeAndCircleContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		EdgeAndCircleContact* pContact = static_cast<EdgeAndCircleContact*>(pAlloc->Allocate(sizeof(EdgeAndCircleContact)));
		new (pContact) EdgeAndCircleContact(pShape0, pShape1);
		return pContact;
	}

	void EdgeAndCircleContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<EdgeAndCircleContact*>(pContact)->~EdgeAndCircleContact();
		pAlloc->Deallocate(pContact, sizeof(EdgeAndCircleContact));
	}

	void EdgeAndCircleContact::Evaluate(Manifold& manifold)
	{
		Collision::EdgeAndCirlce(reinterpret_cast<EdgeShape*>(m_pShape0), m_pShape0->GetBody()->GetTransform(), static_cast<CircleShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), manifold);
	}

	EdgeAndCircleContact::EdgeAndCircleContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	EdgeAndCircleContact::~EdgeAndCircleContact()
	{
	}
}
