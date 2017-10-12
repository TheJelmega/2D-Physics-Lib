#include "Physics/Collision.h"

namespace P2D {

	void Collision::CircleAndCircle(CircleShape* pCircle0, const Transform& tr0, CircleShape* pCircle1, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPoints = 0;

		f32v2 pos0 = tr0.Move(pCircle0->m_RelPos);
		f32v2 pos1 = tr1.Move(pCircle1->m_RelPos);

		f32 distance = pos0.SqDistance(pos1);
		f32 maxDist = pCircle0->m_Radius + pCircle1->m_Radius;
		maxDist *= maxDist;

		if (distance > maxDist)
			return;

		manifold.numPoints = 2;
		manifold.points[0].position = pos0;
		manifold.points[0].normal = f32v2::Zero;

		manifold.points[0].position = pos1;
		manifold.points[0].normal = f32v2::Zero;

	}
}