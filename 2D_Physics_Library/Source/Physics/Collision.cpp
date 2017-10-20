#include "Physics/Collision.h"

#include "Physics/Manifold.h"

#include "Physics/Shapes/CircleShape.h"
#include "Physics/Shapes/EdgeShape.h"

namespace P2D {

	void Collision::CircleAndCircle(CircleShape* pCircle0, const Transform& tr0, CircleShape* pCircle1, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		f32v2 pos0 = tr0.Move(pCircle0->m_RelPos);
		f32v2 pos1 = tr1.Move(pCircle1->m_RelPos);

		f32v2 delta = pos1 - pos0;
		f32 distance = delta.SqLength();
		f32 maxDist = pCircle0->m_Radius + pCircle1->m_Radius;
		maxDist *= maxDist;

		if (distance > maxDist)
			return;

		f32v2 normal = delta / sqrt(distance);

		manifold.numPairs = 1;
		ManifoldPair& pair = manifold.pairs[0];
		pair.normal = normal;
		pair.position0 = pos0 + normal * pCircle0->m_Radius; // Location on circle 1 where hit should have happened
		pair.position1 = pos1 - normal * pCircle1->m_Radius; // Location on circle 2 where hit should have happened
		pair.separation = normal.Dot(pair.position1 - pair.position0);
	}

	void Collision::EdgeAndCirlce(EdgeShape* pEdgeShape, const Transform& tr0, CircleShape* pCircle, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		f32v2 pos = tr1.Move(pCircle->m_RelPos);

		f32v2 v0 = pEdgeShape->m_RelPos + pEdgeShape->m_V0;
		v0 = tr0.Move(v0);
		f32v2 v1 = pEdgeShape->m_RelPos + pEdgeShape->m_V1;
		v1 = tr0.Move(v1);
		f32v2 v1v0 = v1 - v0;
		f32v2 tangent = v1v0.Normalized();

		f32 maxDist = pEdgeShape->m_Radius + pCircle->m_Radius;
		//Find barycentric coordinates
		f32v2 v0Circle = pos - v0;
		f32 u = v0Circle.Dot(tangent);

		if (u < -maxDist)
			return;

		f32v2 circleV1 = v1 - pos;
		f32 v = circleV1.Dot(tangent);

		if (v < -maxDist)
			return;

		f32v2 closestPoint;
		if (u < 0.f)
			closestPoint = v0;
		else if (v < 0.f)
			closestPoint = v1;
		else
			closestPoint = v0 + tangent * u;

		f32 dist = (pos - closestPoint).SqLength();
		maxDist *= maxDist;
		if (dist > maxDist)
			return;

		f32v2 normal = pEdgeShape->m_Normal.Rotated(tr0.rotation);

		manifold.numPairs = 1;
		ManifoldPair& pair = manifold.pairs[0];
		pair.normal = normal;
		pair.position0 = closestPoint;
		pair.position1 = pos - normal * pCircle->m_Radius;
		pair.separation = normal.Dot(pair.position1 - pair.position0);

	}
}
