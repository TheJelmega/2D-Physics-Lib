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
		f32 mass;			/**< Mass of the body*/
		f32 invMass;		/**< Inverse of the mass of the body*/
		f32v2 centerOfMass;	/**< Center of mass of the body*/
		f32 inertia;		/**< Angular inertia of the body*/
		f32 invInertia;		/**< Inverse of the angular inertia of the body*/

		BodyMassData();
	};

	struct BodyDef
	{
		BodyType type;	/**< Body type*/
		f32v2 position;	/**< Position*/
		f32 angle;		/**< Angle*/
		bool active;	/**< Whether the body is active*/
		bool awake;		/**< Whether the body is awake*/

		BodyDef();
	};
	
	class Body
	{
	public:
		Body(const BodyDef& def, World* world);
		~Body();

		/**
		 * Add a shape to the body
		 * @param[in] pShape	Shape to add
		 */
		void AddShape(Shape* pShape);

		/**
		 * Apply a force to the body
		 * @param[in] force	Force to apply on the body
		 * @param[in] wake	Whether to wake the body
		 */
		void ApplyForce(f32v2 force, bool wake = true);
		/**
		 * Apply a force to a point on the body
		 * @param[in] force	Force to apply on the body
		 * @param[in] point	Point where force is applied
		 * @param[in] wake	Whether to wake the body
		 */
		void ApplyForce(f32v2 force, f32v2 point, bool wake = true);
		/**
		 * Apply a torque to the body
		 * @param[in] torque	Torque to apply on the body
		 * @param[in] wake		Whether to wake the body
		 */
		void ApplyTorque(f32 torque, bool wake = true);

		/**
		 * Apply a linear impulse to the body
		 * @param[in] impulse	Linear impulse to apply on the body
		 * @param[in] wake		Whether to wake the body
		 */
		void ApplyImpulse(f32v2 impulse, bool wake = true);
		/**
		 * Apply a linear impulse to a point on the body
		 * @param[in] impulse	Linear impulse to apply on the body
		 * @param[in] point		Point where impulse is applied
		 * @param[in] wake		Whether to wake the body
		 */
		void ApplyImpulse(f32v2 impulse, f32v2 point, bool wake = true);
		/**
		 * Apply an angular impulse to the body
		 * @param[in] impulse	Angular impulse to apply on the body
		 * @param[in] wake		Whether to wake the body
		 */
		void ApplyAngularImpulse(f32 impulse, bool wake = true);

		/**
		 * Get the next body in the list
		 * @return	Next body in the list
		 */
		P2D_FORCE_INL Body* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Body* GetNext() const { return m_pNext; }
		/**
		 * Get the previous body in the list
		 * @return	Previous body in the list
		 */
		P2D_FORCE_INL Body* GetPrev() { return m_pPrev; }
		P2D_FORCE_INL const Body* GetPrev() const { return m_pPrev; }

		/**
		* Get the world
		* @return	World
		*/
		P2D_FORCE_INL World* GetWorld() { return m_pWorld; }
		P2D_FORCE_INL const World* GetWorld() const { return m_pWorld; }

		/**
		 * Get the list of shapes in the body
		 * @return	First shape in the list
		 */
		P2D_FORCE_INL Shape* GetShapes() { return m_pShape; }
		P2D_FORCE_INL const Shape* GetShapes() const { return m_pShape; }
		/**
		 * Get the amount of shapes in the body
		 * @return	Amount of shapes in the body
		 */
		P2D_FORCE_INL u32 GetShapeCount() const { return m_ShapeCount; }

		/**
		 * Get the list of contact nodes in the body
		 * @return	First contact node in the list
		 */
		P2D_FORCE_INL ContactNode* GetContacts() { return m_pContactList; }
		P2D_FORCE_INL const ContactNode* GetContacts() const { return m_pContactList; }

		/**
		 * Get the list of joints nodes in the body
		 * @return	First joints node in the list
		 */
		P2D_FORCE_INL JointNode* GetJoints() { return m_pJointList; }
		P2D_FORCE_INL const JointNode* GetJoints() const { return m_pJointList; }

		/**
		 * Get the AABB of the body
		 * @return	AABB of the body
		 */
		P2D_FORCE_INL AABB GetAABB() const { return m_AABB; }

		/**
		 * Get the body type
		 * @return Body type
		 */
		P2D_FORCE_INL BodyType GetBodyType() const { return m_Type; }
		/**
		 * Get the position of the body
		 * @return	Position of the body
		 */
		P2D_FORCE_INL f32v2 GetPosition() const { return m_Position; }
		/**
		 * Get the angle of the body
		 * @return Angle of the body
		 */
		P2D_FORCE_INL f32 GetAngle() const { return m_Angle; }
		/**
		 * Get the transform of the body
		 * @return	Transform of the body
		 */
		P2D_FORCE_INL Transform& GetTransform() { return m_Transform; }
		P2D_FORCE_INL const Transform& GetTransform() const { return m_Transform; }

		/**
		 * Get the linear velocity of the body
		 * @return	Linear velocity of the body
		 */
		P2D_FORCE_INL f32v2 GetLinearVelocity() const { return m_LinearVelocity; }
		/**
		 * Get the angular velocity of the body
		 * @return	Angular velocity of the body
		 */
		P2D_FORCE_INL f32 GetAngularVelocity() const { return m_AngularVelocity; }
		/**
		 * Get the velocity of the body
		 * @return	Velocity of the body
		 */
		P2D_FORCE_INL const Velocity& GetVelocity() const { return m_Velocity; }

		/**
		 * Get the mass data of the body
		 * @return	Mass data of the body
		 */
		P2D_FORCE_INL const BodyMassData& GetMassData() const { return m_MassData; }

		/**
		 * Get the solver id of the body (mostly for internal use)
		 * @return	Solver id of the body
		 */
		P2D_FORCE_INL i32 GetSolverIndex() const { return m_SolverIndex; }

		/**
		 * Set whether the body is awake
		 * @param[in] awake	Whether the doby should be awake
		 */
		void SetAwake(bool awake);
		/**
		 * Check whether the body is awake
		 * @return	Whether the body is awake
		 */
		P2D_FORCE_INL bool IsAwake() const { return m_Awake; }
		/**
		 * Set whether the body is awake
		 * @param[in] active	Whether the doby should be awake
		 */
		void SetActive(bool active);
		/**
		 * Check whether the body is active
		 * @return	Whether the body is active
		 */
		P2D_FORCE_INL bool IsActive() const { return m_Active; }

		/**
		 * Update the AABB of the body (mostly for internal use)
		 */
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
