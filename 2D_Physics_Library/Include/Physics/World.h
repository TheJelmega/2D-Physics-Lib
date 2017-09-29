#pragma once
#include "Common/BlockAllocator.h"
#include "Body.h"

namespace P2D {
	
	/**
	 * Physics manager
	 */
	class World
	{
	public:
		World();


		Body* CreateBody(const BodyDef& def);
		void DestroyBody(Body* pBody);

		Shape* CreateShape();
		void DestroyShape(Shape* pShape);

		void Update(f32 dt);

	private:
		BlockAllocator m_Allocator;
	};

}
