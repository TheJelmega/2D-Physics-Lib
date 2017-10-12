#pragma once
#include "Shapes/Shape.h"
#include "Math/Math.h"
#include "Common/Settings.h"
#include "Contacts/Contact.h"

namespace P2D {

	class World;
	class CircleShape;

	enum class BodyType
	{
		Dynamic,
		Static,
		Kinematic
	};

	struct BodyMassData
	{
		f32 mass;
		f32v2 centerOfMass;
		f32 inertia;
	};

	struct BodyDef
	{
		BodyType type;
		f32v2 position;
		f32 angle;
		bool active;
		bool awake;

		BodyDef();
	};
	
	class Body
	{
	public:
		Body(const BodyDef& def, World* world);
		~Body();


		void AddShape(Shape* pShape);

		void ApplyForce(f32v2 force, bool wake = true);
		void ApplyForce(f32v2 force, f32v2 point, bool wake = true);
		void ApplyTorque(f32 torque, bool wake = true);

		void ApplyImpulse(f32v2 impulse, bool wake = true);
		void ApplyImpulse(f32v2 impulse, f32v2 point, bool wake = true);
		void ApplyAngularImpulse(f32 impulse, bool wake = true);

		P2D_FORCE_INL Body* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Body* GetNext() const { return m_pNext; }
		P2D_FORCE_INL Body* GetPrev() { return m_pPrev; }
		P2D_FORCE_INL const Body* GetPrev() const { return m_pPrev; }

		P2D_FORCE_INL World* GetWorld() { return m_pWorld; }
		P2D_FORCE_INL const World* GetWorld() const { return m_pWorld; }

		P2D_FORCE_INL Shape* GetShapes() { return m_pShape; }
		P2D_FORCE_INL const Shape* GetShapes() const { return m_pShape; }
		P2D_FORCE_INL u32 GetShapeCount() const { return m_ShapeCount; }

		P2D_FORCE_INL ContactNode* GetContacts() { return m_pContactList; }
		P2D_FORCE_INL const ContactNode* GetContacts() const { return m_pContactList; }

		P2D_FORCE_INL AABB GetAABB() const { return m_AABB; }

		P2D_FORCE_INL BodyType GetBodyType() const { return m_Type; }
		P2D_FORCE_INL f32v2 GetPosition() const { return m_Position; }
		P2D_FORCE_INL f32 GetAngle() const { return m_Angle; }
		P2D_FORCE_INL Transform& GetTransform() { return m_Transform; }
		P2D_FORCE_INL const Transform& GetTransform() const { return m_Transform; }

		P2D_FORCE_INL f32v2 GetLinearVelocity() const { return m_LinearVelocity; }
		P2D_FORCE_INL f32 GetAngularVelocity() const { return m_AngularVelocity; }
		P2D_FORCE_INL f32v2 GetLinearDamping() const { return m_LinearDamping; }
		P2D_FORCE_INL f32 GetAngularDamping() const { return m_AngularDamping; }

		void SetAwake(bool awake);
		P2D_FORCE_INL bool IsAwake() const { return m_Awake; }
		void SetActive(bool active);
		P2D_FORCE_INL bool IsActive() const { return m_Active; }

		void UpdateAABB();
		
	private:
		friend class World;
		friend class Shape;
		friend class CircleShape;
		friend class BroadPhase;
		friend class ContactManager;

		Body* m_pNext;
		Body* m_pPrev;
		World* m_pWorld;

		Shape* m_pShape;
		u32 m_ShapeCount;

		ContactNode* m_pContactList;

		AABB m_AABB;

		BodyType m_Type;

#pragma warning(push)
#pragma warning(disable: 4201) //Disable error C4201 (nameless struct/union)
		union
		{
			Transform m_Transform;
			struct {
				f32v2 m_Position;
				f32 m_Angle;
			};
		};
#pragma warning(pop)

		f32v2 m_LinearVelocity;
		f32 m_AngularVelocity;
		f32v2 m_LinearDamping;
		f32 m_AngularDamping;

		f32v2 m_Force;
		f32 m_Torque;

		BodyMassData m_MassData;

		bool m_Active : 1;
		bool m_Awake : 1;
	};

}
