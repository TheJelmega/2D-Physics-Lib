#pragma once
#include "Math/Math.h"

namespace P2D {
	
	struct ManifoldPair
	{
		f32v2 position0;		/**< Position 0*/
		f32v2 position1;		/**< Position 1*/
		f32v2 normal;			/**< Normal*/
		f32 separation;			/**< Separation*/
	};

	// Structure containing a set of points, used to calculate the collision after detection
	struct Manifold
	{
		ManifoldPair pairs[g_MaxManifoldPairs]; /**< Manifold points*/

		u32 numPairs; /**< Amount of manifold points*/
	};

}