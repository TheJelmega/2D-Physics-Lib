#pragma once
#include "Math/Math.h"
#include "Physics/Material.h"
#include "Math/Transform.h"

namespace P2D {

	class Body;
	class World;
	struct Collision;

	struct MassData
	{
		f32 mass; /**< Mass*/
		f32v2 centerOfMass; /**< Center of mass*/
		f32 shapeInertia; /**< Inertia of the shape, independent of relative position*/
		f32 inertia; /**< Rotational inertia */
		f32 area; /**< Area*/

		MassData();
	};

	struct CollisionFilter
	{
		/**
		 * Collision category
		 */
		u16 category;
		/**
		 * Collision mask, with which groups to collide
		 */
		u16 collisionMask;
		/**
		 * Collision group, always wins over mask\n
		 * 0:	No collision group\n
		 * pos:	Always collides with same group\n
		 * neg:	Never collides with same group
		 */
		i16 group;

		CollisionFilter();
	};

	struct ShapeDef
	{
		f32v2 relpos; /**< Relative position to body*/

		Material material; /**< Physics material*/
		CollisionFilter collisionFilter; /**< Contact filter*/

		bool isSensor; /**< Whether the shape is a sensor*/

		ShapeDef();
	};

	/**
	 * Shape base class
	 */
	class Shape
	{
	public:
		enum class Type
		{
			None,
			Circle,
			Polygon,
			Edge,
			Chain,
			Count
		};


		Shape();
		Shape(Type type, const ShapeDef& def);
		virtual ~Shape();

		/**
		 * Update the mass of the body
		 * @note Should be called after changing the density of the material
		 * @note This function can cause issues when the shape is already added to a body, only use before adding to a body
		 */
		virtual void UpdateMass();
		/**
		 * Update the inertia of the shape (mostly for internal use)
		 */
		virtual void UpdateInertia();
		/**
		 * Set the mass of the shape
		 * @note This function can cause issues when the shape is already added to a body, only use before adding to a body
		 * @param[in] mass	Mass
		 */
		virtual void SetMass(f32 mass);
		/**
		* Update the AABB of the shape (mostly for internal use)
		*/
		virtual void UpdateAABB();

		/**
		 * Get the next shape in the list
		 * @return	Next shape in the list
		 */
		P2D_FORCE_INL Shape* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Shape* GetNext() const { return m_pNext; }

		/**
		 * Get the parent body
		 * @return	Parent body
		 */
		P2D_FORCE_INL Body* GetBody() { return m_pBody; }
		P2D_FORCE_INL const Body* GetBody() const { return m_pBody; }

		/**
		 * Get the shape type
		 * @return	Shape type
		 */
		P2D_FORCE_INL Type GetType() const { return m_Type; }
		/**
		 * Get the AABB of the shape
		 * @return	AABB of the shape
		 */
		P2D_FORCE_INL AABB GetAABB() const { return m_AABB; }
		/**
		 * Get the material of the shape
		 * @return	Material of the shape
		 */
		P2D_FORCE_INL Material& GetMaterial() { return m_Material; }
		P2D_FORCE_INL const Material& GetMaterial() const { return m_Material; }

		/**
		 * Set the relative position of the shape
		 * @param[in] relPos	Relative positition
		 */
		virtual void SetRelPosition(const f32v2& relPos) { m_RelPos = relPos; }
		P2D_FORCE_INL f32v2 GetRelPosition() const { return m_RelPos; }

		/**
		 * Get the mass data of the shape
		 * @return	Mass data of the shape
		 */
		P2D_FORCE_INL const MassData& GetMassData() const { return m_MassData; }
		/**
		 * Get the collision filter of the shape
		 * @return	Collision filter of the shape
		 */
		P2D_FORCE_INL const CollisionFilter& GetFilterData() const { return m_Filter; }
		/**
		 * Check whether the shape is a sensor
		 * @return	Whether the shape is a sensor
		 */
		P2D_FORCE_INL bool IsSensor() const { return m_Sensor; }

	protected:
		friend class Body;
		friend class World;
		friend struct Collision;
		friend class BroadPhase;
		friend class ContactManager;
		friend class Raycaster;
		friend class PhysicsSolver;

		Shape* m_pNext;
		Shape* m_pNextQueried; // Used for raycast
		Body* m_pBody;

		AABB m_AABB;
		Type m_Type;
		Material m_Material;

		f32v2 m_RelPos;
		MassData m_MassData;
		CollisionFilter m_Filter;
		bool m_Sensor;

		f32 m_Radius; // Circle -> radius, other -> skin thickness

		i32 m_ProxyId;
	};

}
