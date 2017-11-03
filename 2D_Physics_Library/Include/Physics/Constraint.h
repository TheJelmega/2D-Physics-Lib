#pragma once
#include "Math/Math.h"

namespace P2D {

	class Body;

	struct ConstraintDef
	{
		bool constrainRotation;
		bool constrainPositionToAxis;
		Body* pBody;

		f32 minAngle;
		f32 maxAngle;

		f32v2 axis;
		f32v2 axisPosition;
		f32 axisTolerance;
		f32 axisMinValue;
		f32 axisMaxValue;

		ConstraintDef();
	};
	
	class Constraint
	{
	public:
		Constraint(const ConstraintDef& def);
		~Constraint();

		void Update();

	private:
		friend class World;
		friend class PhysicsSolver;

		Body* m_pBody;
		Constraint* m_pNext;
		Constraint* m_pPrev;

		bool m_ConstrainRotation;
		bool m_ConstrainPositionToAxis;
		f32 m_MinAngle;
		f32 m_MaxAngle;
		f32v2 m_Axis;
		f32v2 m_AxisPos;
		f32 m_AxisTolerance;
		f32 m_AxisMin;
		f32 m_AxisMax;
	};

}