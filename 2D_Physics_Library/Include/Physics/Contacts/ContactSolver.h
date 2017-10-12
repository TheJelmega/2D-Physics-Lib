#pragma once
#include "Math/Math.h"

namespace P2D {
	
	class ContactManager;

	class ContactSolver
	{
	public:
		ContactSolver();

		void SolveVelocity(f32 dt);
		void SolvePosition(f32 dt);

	private:

		

		ContactManager* m_pContactManager;
	};
}
