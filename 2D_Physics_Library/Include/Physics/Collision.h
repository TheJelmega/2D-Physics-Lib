#pragma once

namespace P2D {

	class CircleShape;
	class EdgeShape;

	struct Transform;
	struct Manifold;
	
	/**
	 * Collision algorithms
	 * @note Collision is a struct, so it can be friended
	 */
	struct Collision {
		
		static void CircleAndCircle(CircleShape* pCircle0, const Transform& tr0, CircleShape* pCircle1, const Transform& tr1, Manifold& manifold);

		static void EdgeAndCirlce(EdgeShape* pEdgeShape, const Transform& tr0, CircleShape* pCircle, const Transform& tr1, Manifold& manifold);
	};

}
