#pragma once
#include "Shapes/CircleShape.h"
#include "Manifold.h"

namespace P2D {
	
	/**
	 * Collision algorithms
	 * @note Collision is a struct, so it can be friended
	 */
	struct Collision {
		
		static void CircleAndCircle(CircleShape* pCircle0, const Transform& tr0, CircleShape* pCircle1, const Transform& tr1, Manifold& manifold);

	};

}
