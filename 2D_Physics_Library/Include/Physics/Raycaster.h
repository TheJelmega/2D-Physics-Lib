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
		f32v2 position;
		f32v2 direction;
		f32 length;

		RaycastInput();
	};

	struct RaycastOutput
	{
		bool hit;
		Shape* pShape;
		f32 fraction;
		f32v2 normal;

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