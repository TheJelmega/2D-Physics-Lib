#include "Physics/Body.h"

namespace P2D {

	BodyDef::BodyDef()
		: type(BodyType::Dynamic)
	{
	}

	Body::Body(const BodyDef& def) 
	: m_pNext(nullptr)
	, m_pPrev(nullptr)
	, m_pShape(nullptr)
	, m_ShapeCount(0)
	{
	}

	Body::~Body()
	{
	}

	void Body::AddShape(Shape* pShape)
	{
		if (m_pShape)
			pShape->m_pNext = m_pShape;

		m_pShape = pShape;
	}
}
