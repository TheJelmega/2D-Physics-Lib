#pragma once
#include "Common/Settings.h"

namespace P2D {
	
	class Contact;

	typedef void(*OnCollisionEnterFunc)(Contact* pContact);
	typedef void(*OnCollisionStayFunc)(Contact* pContact);
	typedef void(*OnCollisionLeaveFunc)(Contact* pContact);
	typedef void(*OnContactCreateFunc)(Contact* pContact);
	typedef void(*OnContactDestroyFunc)(Contact* pContact);

	class EventListener
	{
	public:
		EventListener();

		void SetOnCollisionEnterCallback(OnCollisionEnterFunc onCollisionEnter) { m_OnCollisionEnter = onCollisionEnter; }
		void SetOnCollisionStayCallback(OnCollisionStayFunc onCollisionStay) { m_OnCollisionStay = onCollisionStay; }
		void SetOnCollisionLeaveCallback(OnCollisionLeaveFunc onCollisionLeave) { m_OnCollisionLeave = onCollisionLeave; }
		void SetOnContactCreateCallback(OnCollisionLeaveFunc onContactCreate) { m_OnContactCreate = onContactCreate; }
		void SetOnContactDestroyCallback(OnCollisionLeaveFunc onContactDestroy) { m_OnContactDestroy = onContactDestroy; }

		P2D_FORCE_INL void OnCollisionEnter(Contact* pContact) { if (m_OnCollisionEnter) m_OnCollisionEnter(pContact); }
		P2D_FORCE_INL void OnCollisionStay(Contact* pContact) { if (m_OnCollisionStay) m_OnCollisionStay(pContact); }
		P2D_FORCE_INL void OnCollisionLeave(Contact* pContact) { if (m_OnCollisionLeave) m_OnCollisionLeave(pContact); }
		P2D_FORCE_INL void OnContactCreate(Contact* pContact) { if (m_OnContactCreate) m_OnContactCreate(pContact); }
		P2D_FORCE_INL void OnContactDestroy(Contact* pContact) { if (m_OnContactDestroy) m_OnContactDestroy(pContact); }

	private:
		OnCollisionEnterFunc m_OnCollisionEnter;
		OnCollisionStayFunc m_OnCollisionStay;
		OnCollisionLeaveFunc m_OnCollisionLeave;
		OnContactCreateFunc m_OnContactCreate;
		OnContactDestroyFunc m_OnContactDestroy;
	};

}
