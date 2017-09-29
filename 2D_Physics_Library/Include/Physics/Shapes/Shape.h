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
		virtual ~Shape();

		P2D_FORCE_INL Shape* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Shape* GetNext() const { return m_pNext; }

		P2D_FORCE_INL Type GetType() const { return m_Type; }

	private:
		friend class Body;

		Shape* m_pNext;

		AABB m_AABB;
		Type m_Type;
		Material m_Material;
	};

}
