#include "Physics/World.h"

namespace P2D {

	World::World()
	{
	}

	Body* World::CreateBody(const BodyDef& def)
	{
		Body* pBody = static_cast<Body*>(m_Allocator.Allocate(sizeof(Body)));
		new (pBody) Body(def);
		return pBody;
	}

	void World::DestroyBody(Body* pBody)
	{
		//TODO: Delete children/shapes/...

		//Remove body from body list
		if (pBody->m_pNext)
			pBody->m_pNext->m_pPrev = pBody->m_pPrev;
		if (pBody->m_pPrev)
			pBody->m_pPrev->m_pNext = pBody->m_pNext;

		//Delete body
		pBody->~Body();
		m_Allocator.Deallocate(pBody, sizeof(pBody));
	}

	void World::Update(f32 dt)
	{

	}
}
