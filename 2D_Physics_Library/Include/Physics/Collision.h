#pragma once
#include "Math/Math.h"

namespace P2D {
	class PolygonShape;

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

		static void EdgeAndCirlce(EdgeShape* pEdge, const Transform& tr0, CircleShape* pCircle, const Transform& tr1, Manifold& manifold);

		static void PolgonAndCircle(PolygonShape* pPoly, const Transform& tr0, CircleShape* pCircle, const Transform& tr1, Manifold& manifold);

		static void EdgeAndPolygon(EdgeShape* pEdge, const Transform& tr0, PolygonShape* pPoly, const Transform& tr1, Manifold& manifold);
		
		static void PolygonAndPolygon(PolygonShape* pPoly0, const Transform& tr0, PolygonShape* pPoly1, const Transform& tr1, Manifold& manifold);

		//Utils
		struct PointLinePair
		{
			u8 lineShapeIndex; /**< Shape containing line segment*/
			u32 lineIndex;
			f32v2 point;
			f32 penetration;
		};
	};

}
