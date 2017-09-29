#pragma once
#include "Common/BaseTypes.h"
#include "Vec2.h"

namespace P2D {
	
	/**
	 * AABB (Axis Aligned Bounding Box)
	 */
	struct AABB
	{
		f32v2 min;
		f32v2 max;

		/**
		 * Create an empty AABB
		 */
		AABB();
		/**
		 * Create a AABB from a min and max
		 * @param[in] min	Min
		 * @param[in] max	Max
		 */
		AABB(const f32v2& min, const f32v2& max);
		/**
		 * Create an AABB from values
		 * @param[in] left		Left
		 * @param[in] bottom	Bottom
		 * @param[in] right		Right
		 * @param[in] top		Top
		 */
		AABB(f32 left, f32 bottom, f32 right, f32 top);

		AABB(const AABB& aabb);
		AABB(AABB&& aabb) noexcept;

		AABB& operator=(const AABB& aabb);
		AABB& operator=(AABB&& aabb) noexcept;

		/**
		 * Pad the AABB (extend size)
		 * @value	Value to pad with
		 */
		void Pad(f32 value);
	};

}
