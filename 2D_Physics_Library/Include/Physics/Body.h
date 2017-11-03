#pragma once
#include "Shapes/Shape.h"
#include "Math/Math.h"
#include "Common/Settings.h"
#include "Contacts/Contact.h"
#include "Joints/Joint.h"

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
		f32 invMass;
		f32v2 centerOfMass;
		f32 inertia;
		f32 invInertia;

		BodyMassData();
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
		P2D_FORCE_INL const Velocity& GetVelocity() const { return m_Velocity; }

		P2D_FORCE_INL const BodyMassData& GetMassData() const { return m_MassData; }

		P2D_FORCE_INL i32 GetSolverIndex() const { return m_SolverIndex; }

		void SetAwake(bool awake);
		P2D_FORCE_INL bool IsAwake() const { return m_Awake; }
		void SetActive(bool active);
		P2D_FORCE_INL bool IsActive() const { return m_Active; }

		void UpdateAABB();
		
	private:
		friend class World;
		friend class Shape;
		friend class CircleShape;
		friend class EdgeShape;
		friend class ChainShape;
		friend class PolygonShape;
		friend class BroadPhase;
		friend class ContactManager;
		friend class PhysicsSolver;
		friend struct Collision;
		friend class Constraint;
		friend class Joint;
		friend class RevoluteJoint;
		friend class DistanceJoint;
		friend class FixedJoint;
		friend class PrismaticJoint;
		friend class Raycaster;

		Body* m_pNext;
		Body* m_pPrev;
		World* m_pWorld;

		Shape* m_pShape;
		u32 m_ShapeCount;

		ContactNode* m_pContactList;
		JointNode* m_pJointList;

		AABB m_AABB;

		BodyType m_Type;

#pragma warning(push)
#pragma warning(disable: 4201) //Disable error C4201 (nameless struct/union)
		union
		{
			Transform m_Transform;
			struct 
			{
				f32v2 m_Position;
				f32 m_Angle;
			};
		};
		union
		{
			Velocity m_Velocity;
			struct
			{
				f32v2 m_LinearVelocity;
				f32 m_AngularVelocity;
			};
		};

#pragma warning(pop)

		f32v2 m_Force;
		f32 m_Torque;

		BodyMassData m_MassData;

		i32 m_SolverIndex;
		f32 m_SleepTimer;

		bool m_Active;
		bool m_Awake;
		bool m_InSolver;
	};

}
