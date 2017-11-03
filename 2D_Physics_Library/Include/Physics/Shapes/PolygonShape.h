#pragma once
#include "Shape.h"


namespace P2D {
	class BlockAllocator;

	struct PolygonShapeDef : ShapeDef
	{
		u32 numPoints;
		f32v2* points;

		PolygonShapeDef();
	};
	
	class PolygonShape : public Shape
	{
	public:
		PolygonShape(const PolygonShapeDef& def, BlockAllocator* pAlloc);
		~PolygonShape() override;

		void SetAsBox(f32 width, f32 height);
		void SetAsRegularPolygon(u32 numSides, f32 radius);
			
		void UpdateMass() override;
		void SetMass(f32 mass) override;
		void UpdateInertia() override;
		void UpdateAABB() override;
		void SetRelPosition(const f32v2& relPos) override;

		bool CheckWinding();

		P2D_FORCE_INL u32 GetNumPoints() const { return m_NumPoints; }
		P2D_FORCE_INL const f32v2* GetPoints() const { return m_Points; }

	private:
		friend struct Collision;
		friend class Raycaster;

		f32 CalculateArea();
		f32 CalculateInertiaOVerMass();
		f32v2 CalculateCenterOfMass();
		
		BlockAllocator* m_pAlloc;
		u32 m_NumPoints;
		f32v2* m_Points;
		f32v2* m_Normals;
	};

}
