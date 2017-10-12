#include "Physics/EventListener.h"

namespace P2D {


	EventListener::EventListener()
		: m_OnCollisionEnter(nullptr)
	    , m_OnCollisionStay(nullptr)
	    , m_OnCollisionLeave(nullptr)
	{
	}
}
