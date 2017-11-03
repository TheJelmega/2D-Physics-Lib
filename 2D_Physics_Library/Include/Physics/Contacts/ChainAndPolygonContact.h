#pragma once
#include "Contact.h"
#include "Physics/Shapes/PolygonShape.h"
#include "Physics/Shapes/ChainShape.h"
#include "Physics/Body.h"
#include "Physics/Collision.h"

namespace P2D {

	class ChainAndPolygonContact : public Contact
	{
	public:
		// Moved code to header, since code in cpp cause 'LNK 2019: unresolved error', surprisingly, only this class has the issue
		static Contact* Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc);

		static void Destroy(Contact* pContact, BlockAllocator* pAlloc);

		void Evaluate(Manifold& manifold) override;

	private:

		ChainAndPolygonContact(Shape* pShape0, Shape* pShape1);
		~ChainAndPolygonContact() override;
	};

}
