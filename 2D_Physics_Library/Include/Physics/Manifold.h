#pragma once
#include "Math/Math.h"

namespace P2D {
	
	struct ManifoldPoint
	{
		f32v2 position;		/**< Point position*/
		f32v2 normal;		/**< Point normal*/
	};

	// Structure containing a set of points, used to calculate the collision after detection
	struct Manifold
	{
		enum class Type
		{
			Circles			/**< 2 points, centers of circles*/
		};

		ManifoldPoint points[g_MaxManifoldPoints]; /**< manifold points*/

		u32 numPoints;
	};

}