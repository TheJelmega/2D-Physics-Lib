#pragma once
#include "Math/Math.h"

namespace P2D {
	class PolygonShape;
	class ChainShape;
	class EdgeShape;
	class CircleShape;

	class World;
	class Shape;

	struct RaycastInput
	{
		f32v2 position;		/**< Ray starting position*/
		f32v2 direction;	/**< Ray direction*/
		f32 length;			/**< Ray length*/

		RaycastInput();
	};

	struct RaycastOutput
	{
		bool hit;		/**< Whether the ray hit*/
		Shape* pShape;	/**< Shape the ray hit*/
		f32 fraction;	/**< Fraction of ray length to hit*/
		f32v2 normal;	/**< Normal at hit*/

		RaycastOutput();
	};

	class Raycaster
	{
	public:
		Raycaster(World* pWorld);

		void Raycast(const RaycastInput& input, RaycastOutput& output);

	private:

		void RayCastCircle(CircleShape* pShape, const RaycastInput& input, RaycastOutput& output);
		void RayCastEdge(EdgeShape* pShape, const RaycastInput& input, RaycastOutput& output);
		void RayCastChain(ChainShape* pShape, const RaycastInput& input, RaycastOutput& output);
		void RayCastPolygon(PolygonShape* pShape, const RaycastInput& input, RaycastOutput& output);

		World* m_pWorld;

	};

}