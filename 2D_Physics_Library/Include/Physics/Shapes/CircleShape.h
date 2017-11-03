#pragma once
#include "Shape.h"

namespace P2D {
	
	struct Collsion;

	struct CircleShapeDef : ShapeDef
	{
		f32 radius;

		CircleShapeDef();
	};

	class CircleShape : public Shape
	{
	public:
		CircleShape(const CircleShapeDef& def);
		~CircleShape() override;

		f32 GetRadius() const { return m_Radius; }

		void UpdateMass() override;
		void SetMass(f32 mass) override;
		void SetRelPosition(const f32v2& relPos) override;
		void UpdateAABB() override;
		void UpdateInertia() override;
	private:
		friend struct Collision;
		friend class Raycaster;
	};

}
