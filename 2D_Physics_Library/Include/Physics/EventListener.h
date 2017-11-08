#pragma once
#include "Common/Settings.h"

namespace P2D {
	
	class Contact;

	typedef void(*OnCollisionEnterFunc)(Contact* pContact);
	typedef void(*OnCollisionStayFunc)(Contact* pContact);
	typedef void(*OnCollisionLeaveFunc)(Contact* pContact);
	typedef void(*OnContactCreateFunc)(Contact* pContact);
	typedef void(*OnContactDestroyFunc)(Contact* pContact);
	typedef bool(*PreSolveFunc)(Contact* contact);

	class EventListener
	{
	public:
		EventListener();

		/**
		 * Set the OnCollisionEnter callback
		 * @param[in] onCollisionEnter	OnCollisionEnter callback
		 */
		void SetOnCollisionEnterCallback(OnCollisionEnterFunc onCollisionEnter) { m_OnCollisionEnter = onCollisionEnter; }
		/**
		* Set the OnCollisionStay callback
		* @param[in] onCollisionStay	OnCollisionStay callback
		*/
		void SetOnCollisionStayCallback(OnCollisionStayFunc onCollisionStay) { m_OnCollisionStay = onCollisionStay; }
		/**
		* Set the OnCollisionLeave callback
		* @param[in] onCollisionLeave	OnCollisionLeave callback
		*/
		void SetOnCollisionLeaveCallback(OnCollisionLeaveFunc onCollisionLeave) { m_OnCollisionLeave = onCollisionLeave; }
		/**
		* Set the OnContactCreate callback
		* @param[in] onContactCreate	OnContactCreate callback
		*/
		void SetOnContactCreateCallback(OnCollisionLeaveFunc onContactCreate) { m_OnContactCreate = onContactCreate; }
		/**
		* Set the OnContactnDestroy callback
		* @param[in] onContactDestroy	OnContactnDestroy callback
		*/
		void SetOnContactDestroyCallback(OnCollisionLeaveFunc onContactDestroy) { m_OnContactDestroy = onContactDestroy; }
		/**
		* Set the PreSolv callback
		* @param[in] preSolve	PreSolv callback
		*/
		void SetPreSolveCallback(PreSolveFunc preSolve) { m_PreSolve = preSolve; }

		/**
		* Run the OnCollisionEnter callback
		*/
		P2D_FORCE_INL void OnCollisionEnter(Contact* pContact) const { if (m_OnCollisionEnter) m_OnCollisionEnter(pContact); }
		/**
		* Run the OnCollisionStay callback
		*/
		P2D_FORCE_INL void OnCollisionStay(Contact* pContact) const { if (m_OnCollisionStay) m_OnCollisionStay(pContact); }
		/**
		* Run the OnCollisionLeave callback
		*/
		P2D_FORCE_INL void OnCollisionLeave(Contact* pContact) const { if (m_OnCollisionLeave) m_OnCollisionLeave(pContact); }
		/**
		* Run the OnContactCreate callback
		*/
		P2D_FORCE_INL void OnContactCreate(Contact* pContact) const { if (m_OnContactCreate) m_OnContactCreate(pContact); }
		/**
		* Run the OnContactDestroy callback
		*/
		P2D_FORCE_INL void OnContactDestroy(Contact* pContact) const { if (m_OnContactDestroy) m_OnContactDestroy(pContact); }
		/**
		* Run the PreSolve callback
		* @return	PreSolve callback result
		*/
		P2D_FORCE_INL bool PreSolve(Contact* pContact) const { if (m_PreSolve) return m_PreSolve(pContact); else return true; }

	private:
		OnCollisionEnterFunc m_OnCollisionEnter;
		OnCollisionStayFunc m_OnCollisionStay;
		OnCollisionLeaveFunc m_OnCollisionLeave;
		OnContactCreateFunc m_OnContactCreate;
		OnContactDestroyFunc m_OnContactDestroy;
		PreSolveFunc m_PreSolve;
	};

}
