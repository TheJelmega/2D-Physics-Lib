#pragma once
#include "Contact.h"

namespace P2D {

	class World;
	
	class ContactManager
	{
	public:
		ContactManager(World* pWorld);
		~ContactManager();

		void AddPair(Shape* pShape0, Shape* pShape1);
		void Destroy(Contact* pContact);
		void UpdateCollisions();

	private:
		friend class World;
		friend class ContactSolver;
		friend class PhysicsSolver;

		Contact* m_pContactList;
		u32 m_ContactCount;

		World* m_pWorld;
	};

}
