#pragma once
#include "Math/Math.h"

namespace P2D {
	
	struct ManifoldPair
	{
		f32v2 position0;		/**< position 0*/
		f32v2 position1;		/**< position 1*/
		f32v2 normal;			/**< normal*/
		f32 separation;			/**< separation*/
	};

	// Structure containing a set of points, used to calculate the collision after detection
	struct Manifold
	{
		ManifoldPair pairs[g_MaxManifoldPairs]; /**< manifold points*/

		u32 numPairs;
	};

}