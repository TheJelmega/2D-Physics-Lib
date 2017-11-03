#include "Physics/Contacts/ChainAndPolygonContact.h"
#include "Physics/Collision.h"
#include "Physics/Shapes/Shape.h"
#include "Physics/Body.h"
#include "Physics/Shapes/EdgeShape.h"
#include "Physics/Shapes/ChainShape.h"
#include "Physics/Shapes/PolygonShape.h"

namespace P2D {
	
	// Disabled, check header for more info
	Contact* ChainAndPolygonContact::Create(Shape* pShape0, Shape* pShape1, BlockAllocator* pAlloc)
	{
		ChainAndPolygonContact* pContact = static_cast<ChainAndPolygonContact*>(pAlloc->Allocate(sizeof(ChainAndPolygonContact)));
		new (pContact) ChainAndPolygonContact(pShape0, pShape1);
		return pContact;
	}

	void ChainAndPolygonContact::Destroy(Contact* pContact, BlockAllocator* pAlloc)
	{
		static_cast<ChainAndPolygonContact*>(pContact)->~ChainAndPolygonContact();
		pAlloc->Deallocate(pContact, sizeof(ChainAndPolygonContact));
	}

	void ChainAndPolygonContact::Evaluate(Manifold& manifold)
	{
		//Collision::EdgeAndPolygon(reinterpret_cast<EdgeShape*>(m_pShape0), m_pShape0->GetBody()->GetTransform(), reinterpret_cast<PolygonShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), manifold);

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
			Collision::EdgeAndPolygon(&edge, pChain->GetBody()->GetTransform(), reinterpret_cast<PolygonShape*>(m_pShape1), m_pShape1->GetBody()->GetTransform(), tempManifold);
			/*if (tempManifold.numPairs > 0) // if numPairs > 0, numPair should be 1, so just copy data
			{
				m_Manifold.pairs[m_Manifold.numPairs] = tempManifold.pairs[0];
				++m_Manifold.numPairs;
			}*/
			if (tempManifold.numPairs > 0)
			{
				for (u32 j = 0; j < tempManifold.numPairs; ++j)
				{
					if (manifold.numPairs >= g_MaxManifoldPairs)
						return;

					manifold.pairs[manifold.numPairs] = tempManifold.pairs[j];
					++manifold.numPairs;
				}
			}
		}
	}

	ChainAndPolygonContact::ChainAndPolygonContact(Shape* pShape0, Shape* pShape1)
		: Contact(pShape0, pShape1)
	{
	}

	ChainAndPolygonContact::~ChainAndPolygonContact()
	{
	}
}
