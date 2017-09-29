#pragma once
#include "Shapes/Shape.h"
#include "Math/Math.h"
#include "Common/Settings.h"

namespace P2D {

	class World;

	enum class BodyType
	{
		Dynamic,
		Static,
		Kinematic
	};

	struct BodyDef
	{
		BodyType type;
		f32v2 position;
		f32 angle;

		BodyDef();
	};
	
	class Body
	{
	public:
		Body(const BodyDef& def);
		~Body();


		void AddShape(Shape* pShape);

		P2D_FORCE_INL Body* GetNext() { return m_pNext; }
		P2D_FORCE_INL const Body* GetNext() const { return m_pNext; }
		P2D_FORCE_INL Body* GetPrev() { return m_pPrev; }
		P2D_FORCE_INL const Body* GetPrev() const { return m_pPrev; }

		P2D_FORCE_INL Shape* GetShapes() { return m_pShape; }
		P2D_FORCE_INL const Shape* GetShapes() const { return m_pShape; }
		P2D_FORCE_INL u32 GetShapeCount() const { return m_ShapeCount; }

		P2D_FORCE_INL BodyType GetBodyType() const { return m_Type; }
		P2D_FORCE_INL f32v2 GetPosition() const { return m_Position; }
		P2D_FORCE_INL f32 GetAngle() const { return m_Angle; }
		P2D_FORCE_INL f32v2 GetLinearVelocity() const { return m_LinearVelocity; }
		P2D_FORCE_INL f32 GetAngularVelocity() const { return m_AngularVelocity; }
		P2D_FORCE_INL f32v2 GetLinearDamping() const { return m_LinearDamping; }
		P2D_FORCE_INL f32 GetAngularDamping() const { return m_AngularDamping; }
		
	private:
		friend class ::P2D::World;

		Body* m_pNext;
		Body* m_pPrev;

		Shape* m_pShape;
		u32 m_ShapeCount;

		BodyType m_Type;
		f32v2 m_Position;
		f32 m_Angle;
		f32v2 m_LinearVelocity;
		f32 m_AngularVelocity;
		f32v2 m_LinearDamping;
		f32 m_AngularDamping;

		bool m_Active;
		bool m_Awake;
	};

}
