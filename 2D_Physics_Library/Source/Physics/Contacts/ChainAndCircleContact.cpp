#include "Physics/Contacts/ChainAndCircleContact.h"
#include "Physics/Shapes/ChainShape.h"
#include "Physics/Shapes/EdgeShape.h"
#include "Physics/Shapes/CircleShape.h"
#include "Physics/Collision.h"
#include "Physics/Body.h"

namespace P2D {

	Contact* ChainAndCircleContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		ChainAndCircleContact* pContact = static_cast<ChainAndCircleContact*>(pAlloc->Allocate(sizeof(ChainAndCircleContact)));
		new (pContact) ChainAndCircleContact(pShape0, pShape1);
		return pContact;
	}

	void ChainAndCircleContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<ChainAndCircleContact*>(pContact)->~ChainAndCircleContact();
		pAlloc->Deallocate(pContact, sizeof(ChainAndCircleContact));
	}

	void ChainAndCircleContact::Evaluate(Manifold& manifold)
	{
		EdgeShapeDef edgeDef;
		EdgeShape edge(edgeDef);
		edge.m_pBody = m_pShape0->GetBody();
		ChainShape* pChain = reinterpret_cast<ChainShape*>(m_pShape0);
		Manifold tempManifold;
		m_Manifold.numPairs = 0;
		for (u32 i = 0, count = pChain->m_NumPoints - 1; i < count && manifold.numPairs < g_MaxManifoldPairs; ++i)
		{
			pChain->GetChildEdge(&edge, i);
			if (!edge.m_AABB.Overlaps(m_pShape1->GetAABB())) // Quick AABB check
				continue;
			Collision::EdgeAndCirlce(&edge, pChain->GetBody()->GetTransform(), reinterpret_cast<CircleShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), tempManifold);
			if (tempManifold.numPairs > 0) // if numPairs > 0, numPair should be 1, so just copy data
			{
				if (manifold.numPairs >= g_MaxManifoldPairs)
					return;

				m_Manifold.pairs[m_Manifold.numPairs] = tempManifold.pairs[0];
				++m_Manifold.numPairs;
			}
		}

	}

	ChainAndCircleContact::ChainAndCircleContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	ChainAndCircleContact::~ChainAndCircleContact()
	{
	}
}
