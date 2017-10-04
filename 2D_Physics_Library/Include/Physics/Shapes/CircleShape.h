#pragma once
#include "Physics/Material.h"
#include "Shape.h"

namespace P2D {
	
	struct CircleShapeDef
	{
		Material material;
		f32 radius;
	};

	class CircleShape : public Shape
	{
	public:
		CircleShape(const CircleShapeDef& def);
		~CircleShape() override;

		f32 GetRadius() const { return m_Radius; }

		void UpdateMass() override;
		void SetMass(f32 mass) override;
	private:
		f32 m_Radius;
	};

}
