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
		f32 mass;
		f32v2 centerOfMass;
		/**
		 * Rotational inertia
		 */
		f32 inertia;
		f32 area;

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
		f32v2 relpos;

		Material material;
		CollisionFilter collisionFilter;

		bool isSensor;

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
			Count
		};


		Shape();
		Shape(Type type, const ShapeDef& def);
		virtual ~Shape();

		virtual void UpdateMass();
		virtual void SetMass(f32 mass);
		virtual void UpdateAABB();
		virtual AABB GetAABBAt(const Transform& transform);

		P2D_FORCE_INL Shape* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Shape* GetNext() const { return m_pNext; }

		P2D_FORCE_INL Body* GetBody() { return m_pBody; }
		P2D_FORCE_INL const Body* GetBody() const { return m_pBody; }

		P2D_FORCE_INL Type GetType() const { return m_Type; }
		P2D_FORCE_INL AABB GetAABB() const { return m_AABB; }
		P2D_FORCE_INL Material& GetMaterial() { return m_Material; }
		P2D_FORCE_INL const Material& GetMaterial() const { return m_Material; }

		virtual void SetRelPosition(const f32v2& relPos) { m_RelPos = relPos; }
		P2D_FORCE_INL f32v2 GetRelPosition() const { return m_RelPos; }

		P2D_FORCE_INL const CollisionFilter& GetFilterData() const { return m_Filter; }

		P2D_FORCE_INL bool IsSensor() const { return m_Sensor; }

	protected:
		friend class Body;
		friend class World;
		friend struct Collision;
		friend class BroadPhase;
		friend class ContactManager;

		Shape* m_pNext;
		Body* m_pBody;

		AABB m_AABB;
		Type m_Type;
		Material m_Material;

		f32v2 m_RelPos;
		MassData m_MassData;
		CollisionFilter m_Filter;
		bool m_Sensor;

		f32 m_Radius; /**< Circle -> radius, other -> error from edges*/
	};

}
