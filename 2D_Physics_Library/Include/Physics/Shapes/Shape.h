#pragma once
#include "Math/Math.h"
#include "Physics/Material.h"

namespace P2D {

	class Body;

	
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
			Polygon
		};


		Shape();
		Shape(Type type, const Material& mat);
		virtual ~Shape();

		virtual void UpdateMass();
		virtual void SetMass(f32 mass);

		P2D_FORCE_INL Shape* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Shape* GetNext() const { return m_pNext; }

		P2D_FORCE_INL Type GetType() const { return m_Type; }
		P2D_FORCE_INL AABB GetAABB() const { return m_AABB; }
		P2D_FORCE_INL Material& GetMaterial() { return m_Material; }
		P2D_FORCE_INL const Material& GetMaterial() const { return m_Material; }

	protected:
		friend class Body;

		Shape* m_pNext;

		AABB m_AABB;
		Type m_Type;
		Material m_Material;

		f32 m_Mass;
		f32v2 m_MassCenter;

	};

}
