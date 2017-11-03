#pragma once
#include "Physics/Shapes/Shape.h"
#include "Common/BlockAllocator.h"
#include "Physics/Manifold.h"
#include "Physics/Shapes/CircleShape.h"


namespace P2D {
	
	class Contact;

	struct ContactRegister
	{
		typedef Contact* (*ContactCreateFunc)(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc);
		typedef void (*ContactDestroyFunc)(Contact* contact, BlockAllocator* pAlloc);

		ContactCreateFunc createFunc;
		ContactDestroyFunc destroyFunc;
		bool primary;

		ContactRegister();
	};

	/**
	 * Connection between the contact and a body, stored as a linked list
	 * (name from Box2D
	 */
	struct ContactNode
	{
		Contact* pContact;
		Body* pOther;
		ContactNode* pNext;
		ContactNode* pPrev;

		ContactNode();
	};

	class Contact
	{
	public:
		virtual ~Contact() = default;
		static Contact* Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc);
		static void Destroy(Contact* pContact, BlockAllocator* pAlloc);

		P2D_FORCE_INL const Contact* GetNextTouching() const { return m_pNextTouching; }

		P2D_FORCE_INL Shape* GetShape0() { return m_pShape0; }
		P2D_FORCE_INL const Shape* GetShape0() const { return m_pShape0; }
		P2D_FORCE_INL Shape* GetShape1() { return m_pShape1; }
		P2D_FORCE_INL const Shape* GetShape1() const { return m_pShape1; }

		P2D_FORCE_INL const Manifold& GetManifold() const { return m_Manifold; }
		P2D_FORCE_INL bool IsActive() const { return m_Active; }

		virtual void Evaluate(Manifold& manifold);

	protected:
		friend class ContactManager;
		friend class ContactSolver;
		friend class PhysicsSolver;
		friend class World;

		static void RegisterPair(Shape::Type type0, Shape::Type type1, ContactRegister::ContactCreateFunc createFunc, ContactRegister::ContactDestroyFunc destroyFunc);
		static void InitializeRegisters();

		Contact();
		Contact(Shape* pShape0, Shape* pShape1);

		void Update();
		
		static ContactRegister m_ContactRegisters[i32(Shape::Type::Count)][i32(Shape::Type::Count)];
		static bool m_Initialized;

		Contact* m_pNext;
		Contact* m_pPrev;
		Contact* m_pNextTouching;

		Shape* m_pShape0;
		Shape* m_pShape1;

		ContactNode m_Node0;
		ContactNode m_Node1;

		// Indices for chains
		i32 m_Index0;
		i32 m_Index1;

		Manifold m_Manifold;

		bool m_Touching;
		bool m_CheckFilter;
		bool m_InSolver;
		bool m_Active;
	};

}
