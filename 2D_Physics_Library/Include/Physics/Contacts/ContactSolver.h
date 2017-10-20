#pragma once
#include "Math/Math.h"

namespace P2D {
	
	class PhysicsSolver;

	class ContactSolver
	{
	public:
		ContactSolver(PhysicsSolver* pPhysicsSolver);

		void SolveVelocity(f32 dt);
		void SolvePosition(f32 dt);

	private:
		PhysicsSolver* m_pPhysicsSolver;

	};
}
