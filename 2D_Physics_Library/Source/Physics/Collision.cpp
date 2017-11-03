#include "Physics/Collision.h"

#include "Physics/Manifold.h"
#include "Physics/Body.h"
#include "Physics/World.h"

#include "Physics/Shapes/CircleShape.h"
#include "Physics/Shapes/EdgeShape.h"
#include "Physics/Shapes/PolygonShape.h"
#include "Common/StackAllocator.h"

namespace P2D {

	void Collision::CircleAndCircle(CircleShape* pCircle0, const Transform& tr0, CircleShape* pCircle1, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		f32v2 pos0 = tr0.Move(pCircle0->m_RelPos);
		f32v2 pos1 = tr1.Move(pCircle1->m_RelPos);

		f32v2 delta = pos1 - pos0;
		f32 distance = delta.SqLength();
		f32 maxDist = pCircle0->m_Radius + pCircle1->m_Radius;
		maxDist *= maxDist;

		if (distance > maxDist)
			return;

		f32v2 normal = delta / sqrt(distance);

		manifold.numPairs = 1;
		ManifoldPair& pair = manifold.pairs[0];
		pair.normal = normal;
		pair.position0 = pos0 + normal * pCircle0->m_Radius; // Location on circle 1 where hit should have happened
		pair.position1 = pos1 - normal * pCircle1->m_Radius; // Location on circle 2 where hit should have happened
		pair.separation = normal.Dot(pair.position1 - pair.position0);
	}

	void Collision::EdgeAndCirlce(EdgeShape* pEdge, const Transform& tr0, CircleShape* pCircle, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		f32v2 pos = tr1.Move(pCircle->m_RelPos);

		f32v2 v0 = pEdge->m_RelPos + pEdge->m_V0;
		v0 = tr0.Move(v0);
		f32v2 v1 = pEdge->m_RelPos + pEdge->m_V1;
		v1 = tr0.Move(v1);
		f32v2 v1v0 = v1 - v0;
		f32v2 tangent = v1v0.Normalized();

		f32 maxDist = pEdge->m_Radius + pCircle->m_Radius;
		//Find barycentric coordinates
		f32v2 v0Circle = pos - v0;
		f32 u = v0Circle.Dot(tangent);

		if (u < -maxDist)
			return;

		f32v2 circleV1 = v1 - pos;
		f32 v = circleV1.Dot(tangent);

		if (v < -maxDist)
			return;

		f32v2 closestPoint;
		if (u < 0.f)
			closestPoint = v0;
		else if (v < 0.f)
			closestPoint = v1;
		else
			closestPoint = v0 + tangent * u;

		f32 dist = (pos - closestPoint).SqLength();
		maxDist *= maxDist;
		if (dist > maxDist)
			return;

		f32v2 normal = pEdge->m_Normal.Rotated(tr0.rotation);

		manifold.numPairs = 1;
		ManifoldPair& pair = manifold.pairs[0];
		pair.normal = normal;
		pair.position0 = closestPoint;
		pair.position1 = pos - normal * pCircle->m_Radius;
		pair.separation = normal.Dot(pair.position1 - pair.position0);

	}

	void Collision::PolgonAndCircle(PolygonShape* pPoly, const Transform& tr0, CircleShape* pCircle, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		f32v2 pos = tr1.Move(pCircle->m_RelPos);
		f32 minSeparation = std::numeric_limits<f32>::lowest();
		ManifoldPair curPair;

		//Check each edge
		for (u32 i = 0; i < pPoly->m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == pPoly->m_NumPoints)
				i1 = 0;

			f32v2 v0 = pPoly->m_RelPos + pPoly->m_Points[i];
			v0 = tr0.Move(v0);
			f32v2 v1 = pPoly->m_RelPos + pPoly->m_Points[i1];
			v1 = tr0.Move(v1);
			f32v2 v1v0 = v1 - v0;
			f32v2 tangent = v1v0.Normalized();

			f32 maxDist = pPoly->m_Radius + pCircle->m_Radius;
			//Find barycentric coordinates
			f32v2 v0Circle = pos - v0;
			f32 u = v0Circle.Dot(tangent);

			if (u < -maxDist)
				continue;

			f32v2 circleV1 = v1 - pos;
			f32 v = circleV1.Dot(tangent);

			if (v < -maxDist)
				continue;

			f32v2 closestPoint;
			if (u < 0.f)
				closestPoint = v0;
			else if (v < 0.f)
				closestPoint = v1;
			else
				closestPoint = v0 + tangent * u;

			f32 dist = (pos - closestPoint).SqLength();
			maxDist *= maxDist;
			if (dist > maxDist)
				continue;

			f32v2 normal = (pos - closestPoint) / sqrt(dist);

			ManifoldPair tempPair;
			tempPair.normal = normal;
			tempPair.position0 = closestPoint;
			tempPair.position1 = pos - normal * pCircle->m_Radius;
			tempPair.separation = normal.Dot(tempPair.position1 - tempPair.position0);

			if (tempPair.separation < 0.f && tempPair.separation > minSeparation)
			{
				minSeparation = tempPair.separation;
				curPair = tempPair;
			}
		}

		if (minSeparation > 0.f || minSeparation <= std::numeric_limits<f32>::lowest() + Math::Epsilon<f32>)
			return;

		manifold.numPairs = 1;
		manifold.pairs[0] = curPair;
	}

	// Based on Separating Axis Theorum (SAT)
	void Collision::EdgeAndPolygon(EdgeShape* pEdge, const Transform& tr0, PolygonShape* pPoly, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		StackAllocator* pAlloc = &pEdge->m_pBody->m_pWorld->m_StackAllocator;

		// Find possible penetrating points
		f32v2* possiblePoints = static_cast<f32v2*>(pAlloc->Allocate(pPoly->m_NumPoints * sizeof(f32v2)));
		f32* possiblePenetrations = static_cast<f32*>(pAlloc->Allocate(pPoly->m_NumPoints * sizeof(f32)));
		u32* possiblePointIndices = static_cast<u32*>(pAlloc->Allocate(pPoly->m_NumPoints * sizeof(u32)));
		u32 numPossiblePoints = 0;
		f32v2 edgeV0 = tr0.Move(pEdge->m_RelPos + pEdge->m_V0);
		f32 maxDist = pEdge->m_Radius + pPoly->m_Radius;

		for (u32 i = 0; i < pPoly->m_NumPoints; ++i)
		{
			f32v2 point = tr1.Move(pPoly->m_RelPos + pPoly->m_Points[i]);
			f32v2 dir = point - edgeV0;
			f32 dot = pEdge->m_Normal.Dot(dir);
			if (dot < maxDist)
			{
				possiblePoints[numPossiblePoints] = point;
				possiblePenetrations[numPossiblePoints] = dot;
				possiblePointIndices[numPossiblePoints] = i;
				++numPossiblePoints;
			}
		}

		// if none or all points 'penetrated', all points are below edge
		if (numPossiblePoints == pPoly->m_NumPoints || numPossiblePoints == 0)
		{
			pAlloc->Deallocate(possiblePointIndices);
			pAlloc->Deallocate(possiblePenetrations);
			pAlloc->Deallocate(possiblePoints);
			return;
		}

		//Trim points outside edge and find point(s) with deepest penetration
		f32v2 edgeV1 = tr0.Move(pEdge->m_RelPos + pEdge->m_V1);
		f32v2 edgeTangent = edgeV1 - edgeV0;
		edgeTangent.Normalize();

		constexpr u32 maxPoints = 2;
		f32v2* points = static_cast<f32v2*>(pAlloc->Allocate(maxPoints * sizeof(f32v2)));
		f32* penetrations = static_cast<f32*>(pAlloc->Allocate(maxPoints * sizeof(f32)));
		u32 numPoints = 0;
		f32 minPenetration = maxDist;
		bool polyCheck = false;
		for (u32 i = 0; i < numPossiblePoints; ++i)
		{
			polyCheck = false;
			f32v2 p = possiblePoints[i];

			// Check if point is under the line
			f32v2 v0p = p - edgeV0;
			f32 u = edgeTangent.Dot(v0p);
			if (u < -maxDist)
				polyCheck = true;

			if (!polyCheck)
			{
				f32v2 pv1 = edgeV1 - p;
				f32 v = edgeTangent.Dot(pv1);
				if (v < -maxDist)
					polyCheck = true;
			}

			if (polyCheck)
			{
				polyCheck = false;
				u32 index = possiblePointIndices[i];
				u32 i0 = i32(index) - 1 < 0 ? pPoly->m_NumPoints - 1 : index - 1;
				u32 i1 = index + 1;
				if (i1 > pPoly->m_NumPoints)
					i1 = 0;

				//Check edge 0
				f32v2 v0 = tr1.Move(pPoly->m_RelPos + pPoly->m_Points[i0]);
				f32v2 v1 = tr1.Move(pPoly->m_RelPos + pPoly->m_Points[index]);
				f32v2 n = pPoly->m_Normals[i0].Rotated(tr1.rotation);

				v0p = p - v0;
				f32 penetration = n.Dot(v0p);
				if (penetration > maxDist)
					polyCheck = true;

				if (!polyCheck)
				{
					f32v2 polyTangent = (v1 - v0).Normalized();

					v0p = p - edgeV0;
					u = polyTangent.Dot(v0p);
					if (u < -maxDist)
						polyCheck = true;

					if (!polyCheck)
					{
						f32v2 pv1 = edgeV1 - p;
						f32 v = polyTangent.Dot(pv1);
						if (v < -maxDist)
							polyCheck = true;
					}
				}

				if (polyCheck)
				{
					//Check edge 1
					v0 = tr1.Move(pPoly->m_RelPos + pPoly->m_Points[index]);
					v1 = tr1.Move(pPoly->m_RelPos + pPoly->m_Points[i1]);
					n = pPoly->m_Normals[i0].Rotated(tr1.rotation);

					v0p = p - v0;
					penetration = n.Dot(v0p);
					if (penetration > maxDist)
						continue;

					f32v2 polyTangent = (v1 - v0).Normalized();

					v0p = p - edgeV0;
					u = polyTangent.Dot(v0p);
					if (u < -maxDist)
						continue;

					f32v2 pv1 = edgeV1 - p;
					f32 v = polyTangent.Dot(pv1);
					if (v < -maxDist)
						continue;
				}
			}

			//Check depth
			if (numPoints > 0 && Math::Equals(possiblePenetrations[i], minPenetration))
			{
				if (numPoints == maxPoints)
					break;
				penetrations[numPoints] = possiblePenetrations[i];
				points[numPoints++] = p;
			}
			else if (numPoints == 0 || possiblePenetrations[i] < minPenetration)
			{
				numPoints = 1;
				minPenetration = penetrations[0] = possiblePenetrations[i];
				points[0] = p;
			}
		}

		if (minPenetration > maxDist)
		{
			pAlloc->Deallocate(penetrations);
			pAlloc->Deallocate(points);
			pAlloc->Deallocate(possiblePointIndices);
			pAlloc->Deallocate(possiblePenetrations);
			pAlloc->Deallocate(possiblePoints);
			return;
		}

		//maxDist *= maxDist;

		// Create manifold
		manifold.numPairs = numPoints; // should be 2 or less
		for (u32 i = 0; i < numPoints; ++i)
		{
			//find closest point
			f32v2 v0p = points[i] - edgeV0;
			f32 u = edgeTangent.Dot(v0p);
			f32v2 pv1 = edgeV1 - points[i];
			f32 v = edgeTangent.Dot(pv1);
			f32v2 closestPoint;
			if (u < 0)
				closestPoint = edgeV0;
			if (v < 0)
				closestPoint = edgeV1;
			else
				closestPoint = edgeV0 + edgeTangent * u;

			/*if (penetrations[i] > maxDist)
				penetrations[i] = 0.f;*/

			manifold.pairs[i].position0 = closestPoint;
			manifold.pairs[i].position1 = points[i];
			manifold.pairs[i].normal = pEdge->m_Normal;
			manifold.pairs[i].separation = penetrations[i] > 0.f ? 0.f : penetrations[i];
		}

		pAlloc->Deallocate(penetrations);
		pAlloc->Deallocate(points);
		pAlloc->Deallocate(possiblePointIndices);
		pAlloc->Deallocate(possiblePenetrations);
		pAlloc->Deallocate(possiblePoints);
	}

	// PolygonAndPolygon
	// Using Separating Axis Theorum (SAT), Gilbert-Johnson-Keerthi (GJK) will not works, since it needs disjointed shapes
	void Collision::PolygonAndPolygon(PolygonShape* pPoly0, const Transform& tr0, PolygonShape* pPoly1, const Transform& tr1, Manifold& manifold)
	{
		manifold.numPairs = 0;

		StackAllocator* pAlloc = &pPoly0->m_pBody->m_pWorld->m_StackAllocator;

		u32 size = pPoly0->m_NumPoints * sizeof(f32v2);
		f32v2* points0 = static_cast<f32v2*>(pAlloc->Allocate(size));
		memcpy(points0, pPoly0->m_Points, size);
		f32v2* normals0 = static_cast<f32v2*>(pAlloc->Allocate(size));
		memcpy(normals0, pPoly0->m_Normals, size);
		f32v2* points1 = static_cast<f32v2*>(pAlloc->Allocate(size));
		memcpy(points1, pPoly1->m_Points, size);
		f32v2* normals1 = static_cast<f32v2*>(pAlloc->Allocate(size));
		memcpy(normals1, pPoly1->m_Normals, size);

		// Transform poly0 points
		for (u32 i = 0; i < pPoly0->m_NumPoints; ++i)
		{
			points0[i] = tr0.Move(points0[i]);
			normals0[i].Rotate(tr0.rotation);
		}

		// Transform poly1 points
		for (u32 i = 0; i < pPoly1->m_NumPoints; ++i)
		{
			points1[i] = tr1.Move(points1[i]);
			normals1[i].Rotate(tr1.rotation);
		}

		f32 maxDist = pPoly0->m_Radius + pPoly1->m_Radius;
		PointLinePair* polyPairs = static_cast<PointLinePair*>(pAlloc->Allocate((pPoly0->m_NumPoints + pPoly1->m_NumPoints) * sizeof(PointLinePair)));
		u32 numPolyPairs = 0;

		// Find points of poly 1 in poly 0
		for (u32 i = 0; i < pPoly0->m_NumPoints; ++i)
		{
			f32v2 p0 = points0[i];
			f32v2 n = normals0[i];
			// Iterate through all points of poly 1 for the first time
			if (i == 0)
			{
				for (u32 j = 0; j < pPoly1->m_NumPoints; ++j)
				{
					f32v2 dir = points1[j] - p0;
					f32 dot = n.Dot(dir);
					if (dot < maxDist)
					{
						PointLinePair& pair = polyPairs[numPolyPairs++];
						pair.lineShapeIndex = 0;
						pair.lineIndex = i;
						pair.point = points1[j];
						pair.penetration = dot;
					}
				}
			}
			// Otherwise go through possible points
			else
			{
				u32 count = 0;
				for (u32 j = 0; j < numPolyPairs; ++j)
				{
					PointLinePair& pair = polyPairs[j];
					f32v2 dir = pair.point - p0;
					f32 dot = n.Dot(dir);
					if (dot < maxDist)
					{
						PointLinePair& curPair = polyPairs[count];
						if (dot > pair.penetration)
						{
							curPair.lineIndex = i;
							curPair.penetration = dot;
						}
						else
						{
							curPair.lineIndex = pair.lineIndex;
							curPair.penetration = pair.penetration;
						}
						curPair.point = pair.point;
						++count;
					}
				}
				numPolyPairs = count;
			}
		}
		u32 shape1Pairs = numPolyPairs;

		// Find points of poly 0 in poly 1
		for (u32 i = 0; i < pPoly0->m_NumPoints; ++i)
		{
			f32v2 p1 = points1[i];
			f32v2 n = normals1[i];
			// Iterate through all points of poly 1 for the first time
			if (i == 0)
			{
				for (u32 j = 0; j < pPoly0->m_NumPoints; ++j)
				{
					f32v2 dir = points0[j] - p1;
					f32 dot = n.Dot(dir);
					if (dot < maxDist)
					{
						PointLinePair& pair = polyPairs[numPolyPairs++];
						pair.lineShapeIndex = 1;
						pair.lineIndex = i;
						pair.point = points0[j];
						pair.penetration = dot;
					}
				}
			}
			// Otherwise go through possible points
			else
			{
				u32 count = shape1Pairs;
				for (u32 j = shape1Pairs; j < numPolyPairs; ++j)
				{
					PointLinePair& pair = polyPairs[j];
					f32v2 dir = pair.point - p1;
					f32 dot = n.Dot(dir);
					if (dot < maxDist)
					{
						PointLinePair& curPair = polyPairs[count];
						if (dot > pair.penetration)
						{
							curPair.lineIndex = i;
							curPair.penetration = dot;
						}
						else
						{
							curPair.lineIndex = pair.lineIndex;
							curPair.penetration = pair.penetration;
						}
						curPair.lineShapeIndex = 1;
						curPair.point = pair.point;
						++count;
					}
				}
				numPolyPairs = count;
			}
		}

		//Check if any posible pair exists
		if (numPolyPairs == 0)
		{
			pAlloc->Deallocate(polyPairs);
			pAlloc->Deallocate(normals1);
			pAlloc->Deallocate(points1);
			pAlloc->Deallocate(normals0);
			pAlloc->Deallocate(points0);
			return;
		}

		//process pairs, find deepest penetrations
		constexpr u32 maxPairs = 2;
		PointLinePair* resultingPairs = static_cast<PointLinePair*>(pAlloc->Allocate(maxPairs * sizeof(PointLinePair)));
		u32 numResultingPairs = 0;
		f32 minPenetration = maxDist;
		for (u32 i = 0; i < numPolyPairs; ++i)
		{
			PointLinePair& pair = polyPairs[i];
			if (numResultingPairs > 0 && Math::Equals(pair.penetration, minPenetration))
			{
				if (numResultingPairs != maxPairs)
				{
					resultingPairs[numResultingPairs++] = pair;
				}
			}
			else if (numPolyPairs == 0 || pair.penetration < minPenetration)
			{
				resultingPairs[0] = pair;
				numResultingPairs = 1;
				minPenetration = pair.penetration;
			}
		}

		// generate manifold
		// Use normals of first pair
		f32v2 n;
		if (resultingPairs[0].lineShapeIndex == 0)
			n = pPoly0->m_Normals[resultingPairs[0].lineIndex];
		else
			n = -pPoly1->m_Normals[resultingPairs[0].lineIndex];

		manifold.numPairs = numResultingPairs;
		for (u32 i = 0; i < numResultingPairs; ++i)
		{
			ManifoldPair& manifoldPair = manifold.pairs[i];
			PointLinePair& pair = resultingPairs[i];

			if (pair.lineShapeIndex == 0)
			{
				f32v2 p = pair.point;
				manifoldPair.position1 = p;
				u32 index1 = pair.lineIndex + 1;
				if (index1 >= pPoly0->m_NumPoints)
					index1 = 0;
				f32v2 v0 = points0[pair.lineIndex];
				f32v2 v1 = points0[index1];
				f32v2 v0p = p - v0;
				f32v2 tangent = (v1 - v0).Normalize();
				f32 u = tangent.Dot(v0p);
				f32v2 closestPoint = v0 + tangent * u;
				manifoldPair.position0 = closestPoint;
			}
			else
			{
				f32v2 p = pair.point;
				manifoldPair.position0 = p;
				u32 index1 = pair.lineIndex + 1;
				if (index1 >= pPoly1->m_NumPoints)
					index1 = 0;
				f32v2 v0 = points1[pair.lineIndex];
				f32v2 v1 = points1[index1];
				f32v2 v0p = p - v0;
				f32v2 tangent = (v1 - v0).Normalize();
				f32 u = tangent.Dot(v0p);
				f32v2 closestPoint = v0 + tangent * u;
				manifoldPair.position1 = closestPoint;
			}

			manifoldPair.normal = n;
			manifoldPair.separation = resultingPairs[i].penetration;
		}

		pAlloc->Deallocate(resultingPairs);
		pAlloc->Deallocate(polyPairs);
		pAlloc->Deallocate(normals1);
		pAlloc->Deallocate(points1);
		pAlloc->Deallocate(normals0);
		pAlloc->Deallocate(points0);
	}
}
