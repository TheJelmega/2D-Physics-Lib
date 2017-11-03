#pragma once
#include "Contact.h"

namespace P2D {
	
	class EdgeAndCircleContact : public Contact
	{
	public:
		static Contact* Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc);
		static void Destroy(Contact* pContact, BlockAllocator* pAlloc);

		
		void Evaluate(Manifold& manifold) override;

	private:

		EdgeAndCircleContact(Shape* pShape0, Shape* pShape1);
		~EdgeAndCircleContact() override;
	};

}
