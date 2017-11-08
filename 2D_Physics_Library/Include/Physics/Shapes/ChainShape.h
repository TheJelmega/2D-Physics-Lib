#pragma once
#include "Shape.h"
#include "Common/BlockAllocator.h"
#include "EdgeShape.h"

namespace P2D {
	
	struct ChainShapeDef : ShapeDef
	{
		u32 numPoints; /**< Number of points in the chain*/
		f32v2* points; /**< Points in the chain*/

		ChainShapeDef();
	};

	class ChainShape : public Shape
	{
	public:
		ChainShape(const ChainShapeDef& def, BlockAllocator* pAlloc);
		~ChainShape() override;
		void UpdateAABB() override;
		void SetRelPosition(const f32v2& relPos) override;

		/**
		 * Get an edge in the chain
		 * @param[inout] pEdge		Edge to set values to
		 * @param[in] childIndex	Index of the edge in the chain
		 */
		void GetChildEdge(EdgeShape* pEdge, u32 childIndex);

		/**
		 * Get the points in the chain
		 * @return	Points in the chain
		 */
		P2D_FORCE_INL const f32v2* GetPoints() const { return m_Points; }
		/**
		 * Get the number of points in the chain
		 * @return	Number of points in the chain
		 */
		P2D_FORCE_INL u32 GetNumPoints() const { return m_NumPoints; }

	private:
		friend class ChainAndCircleContact;
		friend class ChainAndPolygonContact;
		friend class Raycaster;

		BlockAllocator* m_pAlloc;
		u32 m_NumPoints;
		f32v2* m_Points;
	};

}
