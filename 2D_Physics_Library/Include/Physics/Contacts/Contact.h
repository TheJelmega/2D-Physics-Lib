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
		Contact* pContact;	/**< Contact*/
		Body* pOther;		/**< Other body in contact*/
		ContactNode* pNext;	/**< Next node*/
		ContactNode* pPrev;	/**< Previous node*/

		ContactNode();
	};

	class Contact
	{
	public:
		virtual ~Contact() = default;
		/**
		 * Create a contact (internal use only)
		 */
		static Contact* Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc);
		/**
		* Destroy a contact (internal use only)
		*/
		static void Destroy(Contact* pContact, BlockAllocator* pAlloc);

		/**
		* Get the next contact
		* @return	Next contact
		*/
		P2D_FORCE_INL const Contact* GetNext() const { return m_pNext; }
		/**
		* Get the previous contact
		* @return	Previous contact
		*/
		P2D_FORCE_INL const Contact* GetPrev() const { return m_pPrev; }
		/**
		 * Get the next touching contact
		 * @return	Next touching contact
		 */
		P2D_FORCE_INL const Contact* GetNextTouching() const { return m_pNextTouching; }

		/**
		 * Get shape 0 of the contact
		 * @return Shape 0
		 */
		P2D_FORCE_INL Shape* GetShape0() { return m_pShape0; }
		P2D_FORCE_INL const Shape* GetShape0() const { return m_pShape0; }
		/**
		 * Get shape 1 of the contact
		 * @return Shape 1
		 */
		P2D_FORCE_INL Shape* GetShape1() { return m_pShape1; }
		P2D_FORCE_INL const Shape* GetShape1() const { return m_pShape1; }

		/**
		 * Get the contact manifold
		 * @return	Contact manifold
		 */
		P2D_FORCE_INL const Manifold& GetManifold() const { return m_Manifold; }
		/**
		 * Check whether the contact is touching
		 * @return	Whether the contact is touching
		 */
		P2D_FORCE_INL bool IsTouching() const { return m_Touching; }
		/**
		* Check whether the contact is active
		* @return	Whether the contact is active
		*/
		P2D_FORCE_INL bool IsActive() const { return m_Active; }
		/**
		 * Set whether to recheck the contact filter
		 * @param[in] check	Whether to recheck the contact filter
		 */
		P2D_FORCE_INL void SetCheckFilter(bool check) { m_CheckFilter = check; }

		/**
		 * Evaluate the contact (internal use only)
		 */
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

		Manifold m_Manifold;

		bool m_Touching;
		bool m_CheckFilter;
		bool m_InSolver;
		bool m_Active;
	};

}
