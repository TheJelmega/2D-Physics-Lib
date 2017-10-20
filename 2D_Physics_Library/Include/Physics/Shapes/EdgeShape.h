#pragma once
#include "Shape.h"


namespace P2D {
	
	struct EdgeShapeDef : ShapeDef
	{
		f32v2 v0;
		f32v2 v1;
	};

	class EdgeShape : public Shape
	{
	public:
		EdgeShape(const EdgeShapeDef& def);
		~EdgeShape() override;
		void UpdateAABB() override;
		void SetRelPosition(const f32v2& relPos) override;

		P2D_FORCE_INL const f32v2& GetV0() const { return m_V0; }
		P2D_FORCE_INL const f32v2& GetV1() const { return m_V1; }
		P2D_FORCE_INL const f32v2& GetNormal() const { return m_Normal; }

	private:
		friend struct Collision;
		friend class ChainShape;
		friend class ChainAndCircleContact;

		f32v2 m_V0, m_V1, m_Normal;
	};

}
