#include "Physics/Contacts/ContactSolver.h"
#include "Physics/Contacts/Contact.h"
#include "Physics/Contacts/ContactManager.h"
#include "Physics/PhysicsSolver.h"
#include "Physics/World.h"

namespace P2D {


	ContactSolver::ContactSolver(PhysicsSolver* pPhysicsSolver)
		: m_pPhysicsSolver(pPhysicsSolver)
	{
	}

	void ContactSolver::SolveVelocity(f32 dt)
	{
		(void)dt;
		for (Contact* pContact = m_pPhysicsSolver->m_pTouchingContact; pContact; pContact = pContact->m_pNextTouching)
		{
			Shape* pShape0 = pContact->m_pShape0;
			Shape* pShape1 = pContact->m_pShape1;

			Body* pBody0 = pShape0->GetBody();
			Body* pBody1 = pShape1->GetBody();

			i32 index0 = pBody0->GetSolverIndex();
			i32 index1 = pBody1->GetSolverIndex();

			const Manifold& manifold = pContact->m_Manifold;

			Velocity& vel0 = m_pPhysicsSolver->m_Velocities[index0];
			Velocity& vel1 = m_pPhysicsSolver->m_Velocities[index1];

			Transform& tr0 = m_pPhysicsSolver->m_Transforms[index0];
			Transform& tr1 = m_pPhysicsSolver->m_Transforms[index1];

			const Material& mat0 = pShape0->GetMaterial();
			const Material& mat1 = pShape1->GetMaterial();

			const BodyMassData& mass0 = pBody0->GetMassData();
			const BodyMassData& mass1 = pBody1->GetMassData();

			f32v2 comPos0 = tr0.Move(mass0.centerOfMass);
			f32v2 comPos1 = tr1.Move(mass1.centerOfMass);

			for (u32 i = 0; i < manifold.numPairs; ++i)
			{
				Velocity tvel0 = pBody0->GetVelocity();
				Velocity tvel1 = pBody1->GetVelocity();

				const ManifoldPair& pair = manifold.pairs[i];
				f32v2 rad0 = pair.position0 - comPos0;
				f32v2 rad1 = pair.position1 - comPos1;

				f32v2 pointVel0 = tvel0.linearVelocity + rad0.Cross(tvel0.angularVelocity);
				f32v2 pointVel1 = tvel1.linearVelocity + rad1.Cross(tvel1.angularVelocity);

				f32v2 relVel = pointVel1 - pointVel0;
				f32 normalSpeed = relVel.Dot(pair.normal);

				f32 restitution = m_pPhysicsSolver->CombineRestitution(mat0.restitution, mat1.restitution);

				f32 normalMass = 1.f / (mass0.invMass + mass1.invMass);

				f32 normalImpulse = -(1 + restitution) * normalSpeed;
				normalImpulse *= normalMass;
				normalImpulse /= manifold.numPairs;

				if (normalSpeed <= 0)
				{
					f32v2 impulse = pair.normal * normalImpulse;

					vel0.linearVelocity -= impulse * mass0.invMass;
					vel0.angularVelocity -= rad0.Cross(impulse) * mass0.invInertia;
					tvel0.linearVelocity -= impulse * mass0.invMass;
					tvel0.angularVelocity -= rad0.Cross(impulse) * mass0.invInertia;
					vel1.linearVelocity += impulse * mass1.invMass;
					vel1.angularVelocity += rad1.Cross(impulse) * mass1.invInertia;
					tvel1.linearVelocity += impulse * mass1.invMass;
					tvel1.angularVelocity += rad1.Cross(impulse) * mass1.invInertia;
				}

					//Update Friction
					pointVel0 = tvel0.linearVelocity + rad0.Cross(tvel0.angularVelocity);
					pointVel1 = tvel1.linearVelocity + rad1.Cross(tvel1.angularVelocity);

					f32v2 prevRelVel = relVel;
					relVel = pointVel1 - pointVel0;
					f32v2 tangent = relVel - (pair.normal.Dot(prevRelVel)) * pair.normal;
					tangent.Normalize();
					f32 tangentSpeed = relVel.Dot(tangent);

					f32 staticFriction = m_pPhysicsSolver->CombineStaticFriction(mat0.staticFriction, mat1.staticFriction);
					f32 dynamicFriction = m_pPhysicsSolver->CombineDynamicFriction(mat0.dynamicFriction, mat1.dynamicFriction);

					f32 tangentMass = 1.f / (mass0.invMass + mass0.invInertia * rad0.SqLength() + mass1.invMass + mass1.invInertia * rad1.SqLength());

					f32 tangentForce = -tangentSpeed * tangentMass;
					f32v2 tangentImpulse;

					if (abs(tangentForce) < staticFriction * normalImpulse)
					{
						tangentImpulse = tangentForce * tangent;
					}
					else
					{
						tangentImpulse = -dynamicFriction * normalImpulse * tangent;
					}

					vel0.linearVelocity -= tangentImpulse * mass0.invMass;
					vel0.angularVelocity -= rad0.Cross(tangentImpulse) * mass0.invInertia;
					vel1.linearVelocity += tangentImpulse * mass1.invMass;
					vel1.angularVelocity += rad1.Cross(tangentImpulse) * mass1.invInertia;
				
			}
		}
	}

	void ContactSolver::SolvePosition(f32 dt)
	{
		(void)dt;
 		for (Contact* pContact = m_pPhysicsSolver->m_pTouchingContact; pContact; pContact = pContact->m_pNextTouching)
		{

			Shape* pShape0 = pContact->m_pShape0;
			Shape* pShape1 = pContact->m_pShape1;

			Body* pBody0 = pShape0->GetBody();
			Body* pBody1 = pShape1->GetBody();

			i32 index0 = pBody0->GetSolverIndex();
			i32 index1 = pBody1->GetSolverIndex();

			const Manifold& manifold = pContact->m_Manifold;

			for (u32 i = 0; i < manifold.numPairs; ++i)
			{
				const ManifoldPair& pair = manifold.pairs[i];

				if (pair.separation >= g_LinearSlop)
					continue;

				f32 impulse = pair.separation / manifold.numPairs;

				if (pBody0->GetBodyType() == BodyType::Static)
				{
					m_pPhysicsSolver->m_Transforms[index1].position -= pair.normal * impulse;
				}
				else if (pBody1->GetBodyType() == BodyType::Static)
				{
					m_pPhysicsSolver->m_Transforms[index0].position += pair.normal * impulse;
				}
				else
				{
					m_pPhysicsSolver->m_Transforms[index0].position += pair.normal * impulse * 0.5f;
					m_pPhysicsSolver->m_Transforms[index1].position -= pair.normal * impulse * 0.5f;
				}
			}
		}
	}
}
