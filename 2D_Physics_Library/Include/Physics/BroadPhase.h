#pragma once

namespace P2D {
	
	class World;
	class ContactManager;

	class BroadPhase
	{
	public:
		BroadPhase(World* pWorld);

		/**
		 * Update collsion broadphase
		 * @param[in] callback ContactManager to add pairs to
		 */
		void UpdatePairs(ContactManager* callback);

	private:
		World* m_pWorld;
	};

}