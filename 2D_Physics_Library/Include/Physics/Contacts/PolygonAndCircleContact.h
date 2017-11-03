#pragma once

#pragma once
#include "Contact.h"

namespace P2D {

	class PolygonAndCircleContact : public Contact
	{
	public:
		static Contact* Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc);
		static void Destroy(Contact* pContact, BlockAllocator* pAlloc);


		void Evaluate(Manifold& manifold) override;

	private:

		PolygonAndCircleContact(Shape* pShape0, Shape* pShape1);
		~PolygonAndCircleContact() override;
	};

}
