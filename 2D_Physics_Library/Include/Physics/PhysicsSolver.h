#pragma once
#include "Math/Math.h"
#include "Math/Transform.h"

namespace P2D {
	
	class World;
	class BroadPhase;
	class ContactManager;
	class StackAllocator;
	class Body;
	class Contact;

	struct PhysicsSolverSettings
	{
		u32 numBodies;

		PhysicsSolverSettings();
	};

	enum class ResitutionCombinationType : u8
	{
		Min,
		Max,
		Average,
		Multiply
	};

	enum class FrictionCombinationType : u8
	{
		Min,
		Max,
		Average,
		Multiply
	};

	class PhysicsSolver
	{
	public:
		PhysicsSolver(World* pWorld, StackAllocator* pAllocator);
		~PhysicsSolver();

		void Prepare(const PhysicsSolverSettings& settings);

		void Add(Body* pBody);
		void Add(Contact* pContact);

		//void UpdateCollisions();

		void Solve(f32 dt);

		void Clear();

	private:
		friend class ContactSolver;
		friend class World;

		f32 CombineRestitution(f32 r0, f32 r1) const;
		f32 CombineStaticFriction(f32 f0, f32 f1) const;
		f32 CombineDynamicFriction(f32 f0, f32 f1) const;

		void AddNewBodies();

		World* m_pWorld;
		ContactManager* m_pContactManager;
		StackAllocator* m_pAllocator;

		u32 m_BodyCount;
		u32 m_BodyCapacity;
		u32 m_ContactCount;
		Body** m_pBodies;
		Transform* m_Transforms;
		Velocity* m_Velocities;
		Contact* m_pTouchingContact;
		
		ResitutionCombinationType m_ResitutionCombinationType;
		FrictionCombinationType m_StaticFrictionCombinationType;
		FrictionCombinationType m_DynamicFrictionCombinationType;
	};

}
