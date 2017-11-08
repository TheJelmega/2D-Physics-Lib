#pragma once
#include "Shape.h"


namespace P2D {
	
	struct EdgeShapeDef : ShapeDef
	{
		f32v2 v0; /**< Vertex 0*/
		f32v2 v1; /**< Vertex 1*/
	};

	class EdgeShape : public Shape
	{
	public:
		EdgeShape(const EdgeShapeDef& def);
		~EdgeShape() override;
		void UpdateAABB() override;
		void SetRelPosition(const f32v2& relPos) override;

		/**
		 * Get vertex 0 of the edge
		 * @return	Vertex 0 of the edge
		 */
		P2D_FORCE_INL const f32v2& GetV0() const { return m_V0; }
		/**
		 * Get vertex 1 of the edge
		 * @return	Vertex 1 of the edge
		 */
		P2D_FORCE_INL const f32v2& GetV1() const { return m_V1; }
		/**
		 * Get the normal of the edge
		 * @return	Normal of the edge
		 */
		P2D_FORCE_INL const f32v2& GetNormal() const { return m_Normal; }

	private:
		friend struct Collision;
		friend class ChainShape;
		friend class ChainAndCircleContact;
		friend class ChainAndPolygonContact;
		friend class Raycaster;

		f32v2 m_V0, m_V1, m_Normal;
	};

}
