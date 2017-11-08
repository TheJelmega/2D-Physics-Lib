#pragma once
#include "Shape.h"


namespace P2D {
	class BlockAllocator;

	struct PolygonShapeDef : ShapeDef
	{
		u32 numPoints; /**< Number of points in the polygon*/
		f32v2* points; /**< Points in the polygon*/

		PolygonShapeDef();
	};
	
	class PolygonShape : public Shape
	{
	public:
		PolygonShape(const PolygonShapeDef& def, BlockAllocator* pAlloc);
		~PolygonShape() override;

		/**
		 * Set the polygon as a box
		 * @param[in] width		Width of the box
		 * @param[in] height	Height of the box
		 */
		void SetAsBox(f32 width, f32 height);
		/**
		 * Set the polygon as a regular polygon
		 * @param[in] numSides	Number of sides of the polygon
		 * @param[in] radius	Radius of the polygon
		 */
		void SetAsRegularPolygon(u32 numSides, f32 radius);
			
		void UpdateMass() override;
		void SetMass(f32 mass) override;
		void UpdateInertia() override;
		void UpdateAABB() override;
		void SetRelPosition(const f32v2& relPos) override;

		/**
		 * Check whether the winding is in the correct order (CCW)
		 * @return Whether the winding is correct
		 */
		bool CheckWinding() const;

		/**
		 * Get the points in the polygon
		 * @return	Points in the polygon
		 */
		P2D_FORCE_INL u32 GetNumPoints() const { return m_NumPoints; }
		/**
		 * Get the number of points in the polygon
		 * @return	Number of points in the polygon
		 */
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
