#pragma once
#include "Shape.h"
#include "Common/BlockAllocator.h"
#include "EdgeShape.h"

namespace P2D {
	
	struct ChainShapeDef : ShapeDef
	{
		u32 numPoints;
		f32v2* points;

		ChainShapeDef();
	};

	class ChainShape : public Shape
	{
	public:
		ChainShape(const ChainShapeDef& def, BlockAllocator* pAlloc);
		~ChainShape() override;
		void Dealloc(BlockAllocator* pAlloc);
		void UpdateAABB() override;
		void SetRelPosition(const f32v2& relPos) override;

		void GetChildEdge(EdgeShape* pEdge, u32 childIndex);

		P2D_FORCE_INL const f32v2* GetPoints() const { return m_Points; }
		P2D_FORCE_INL u32 GetNumPoints() const { return m_NumPoints; }

	private:
		friend class ChainAndCircleContact;

		u32 m_NumPoints;
		f32v2* m_Points;
	};

}
