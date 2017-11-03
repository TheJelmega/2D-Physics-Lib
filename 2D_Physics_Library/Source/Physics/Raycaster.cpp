#include "Physics/Raycaster.h"
#include "Physics/World.h"
#include "Physics/BroadPhase.h"

namespace P2D {


	RaycastInput::RaycastInput()
		: length(0)
	{
	}

	RaycastOutput::RaycastOutput()
		: hit(false)
		, pShape(nullptr)
		, fraction(0)
	{
	}

	Raycaster::Raycaster(World* pWorld)
		: m_pWorld(pWorld)
	{
	}

	void Raycaster::Raycast(const RaycastInput& input, RaycastOutput& output)
	{
		
		f32v2 rayEnd = input.position + input.direction * input.length;

		//Create an AABB for the ray
		AABB aabb;
		aabb.max.x = Math::Max(input.position.x, rayEnd.x);
		aabb.max.y = Math::Max(input.position.y, rayEnd.y);
		aabb.min.x = Math::Min(input.position.x, rayEnd.x);
		aabb.min.y = Math::Min(input.position.y, rayEnd.y);

		//Quere shapes
		Shape* pShape = nullptr;
		m_pWorld->m_pBroadPhase->Query(aabb, pShape);

		//Raycast shapes and find closest one
		RaycastOutput temp;
		for (; pShape; pShape = pShape->m_pNextQueried)
		{
			switch (pShape->m_Type)
			{
			case Shape::Type::Circle:
				RayCastCircle(reinterpret_cast<CircleShape*>(pShape), input, temp);
				break;
			case Shape::Type::Polygon:
				RayCastEdge(reinterpret_cast<EdgeShape*>(pShape), input, temp);
				break;
			case Shape::Type::Edge:
				RayCastChain(reinterpret_cast<ChainShape*>(pShape), input, temp);
				break;
			case Shape::Type::Chain:
				RayCastPolygon(reinterpret_cast<PolygonShape*>(pShape), input, temp);
				break;
			default: ;
			}

			if (temp.hit)
			{
				if (temp.fraction < output.fraction || !output.hit)
					output = temp;
			}
		}
	}

	void Raycaster::RayCastCircle(CircleShape* pShape, const RaycastInput& input, RaycastOutput& output)
	{
		output.hit = false;

		Body* pBody = pShape->m_pBody;
		f32v2 circlePos = pBody->m_Transform.Move(pShape->m_RelPos);

		f32v2 rc = circlePos - input.position;
		f32 dot = input.direction.Dot(rc);
		f32v2 closestPoint = input.position + input.direction * dot;

		f32v2 diff = circlePos - closestPoint;
		f32 dist = diff.SqLength();
		f32 radius2 = pShape->m_Radius * pShape->m_Radius;

		if (dist > radius2)
			return;

		f32 offset = sqrt(radius2 - dist);
		f32 l = dot - offset;

		if (l > input.length)
			return;

		output.hit = true;
		output.pShape = pShape;
		output.fraction = l / input.length;
		f32v2 point = input.position + input.direction * l;
		output.normal = (point - circlePos).Normalize();
	}

	void Raycaster::RayCastEdge(EdgeShape* pShape, const RaycastInput& input, RaycastOutput& output)
	{
		output.hit = false;

		Body* pBody = pShape->m_pBody;

		f32v2 v0 = pBody->m_Transform.Move(pShape->m_RelPos + pShape->m_V0);
		f32v2 v1 = pBody->m_Transform.Move(pShape->m_RelPos + pShape->m_V0);
		f32v2 v0v1 = v1 - v0;
		f32v2 rayEnd = input.position + input.direction + input.length;
		f32v2 dir = rayEnd - input.position;

		f32 d = dir.Cross(v0v1);

		if (abs(d) < Math::Epsilon<f32>)
			return;

		f32 pre = input.position.Cross(rayEnd);
		f32 post = v0.Cross(v1);
		f32 x = (pre * (v0.x - v1.x) - (input.position.x - rayEnd.x) * post) / d;
		f32 y = (pre * (v0.y - v1.y) - (input.position.y - rayEnd.y) * post) / d;

		// Check if point is on lines
		if (x < Math::Min(input.position.x, rayEnd.x) || x > Math::Max(input.position.x, rayEnd.x) || y < Math::Min(input.position.y, rayEnd.y) || y > Math::Max(input.position.y, rayEnd.y))
			return;
		if (x < Math::Min(v0.x, v1.x) || x > Math::Max(v0.x, v1.x) || y < Math::Min(v0.y, v1.y) || y > Math::Max(v0.y, v1.y))
			return;

		f32v2 point(x, y);
		f32 l = (input.position - point).Length();

		if (l > input.length)
			return;

		output.hit = true;
		output.pShape = pShape;
		output.normal = pShape->m_Normal;
		output.fraction = l / input.length;
	}

	void Raycaster::RayCastChain(ChainShape* pShape, const RaycastInput& input, RaycastOutput& output)
	{
		output.hit = false;
		P2D::EdgeShapeDef def;
		P2D::EdgeShape edge(def);

		for (u32 i = 0, size = pShape->m_NumPoints - 1; i < size; ++i)
		{
			pShape->GetChildEdge(&edge, i);
			RaycastOutput temp;
			RayCastEdge(&edge, input, temp);

			if (temp.hit)
			{
				if (temp.fraction < output.fraction || !output.hit)
					output = temp;
			}
		}
	}

	void Raycaster::RayCastPolygon(PolygonShape* pShape, const RaycastInput& input, RaycastOutput& output)
	{
		output.hit = false;

		Body* pBody = pShape->m_pBody;
		
		//Check each edge
		for (u32 i = 0; i < pShape->m_NumPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 >= pShape->m_NumPoints)
				i1 = 0;

			f32v2 v0 = pBody->m_Transform.Move(pShape->m_RelPos + pShape->m_Points[i]);
			f32v2 v1 = pBody->m_Transform.Move(pShape->m_RelPos + pShape->m_Points[i1]);
			f32v2 v0v1 = v1 - v0;
			f32v2 rayEnd = input.position + input.direction + input.length;
			f32v2 dir = rayEnd - input.position;

			f32 d = dir.Cross(v0v1);

			if (abs(d) < Math::Epsilon<f32>)
				continue;;

			f32 pre = input.position.Cross(rayEnd);
			f32 post = v0.Cross(v1);
			f32 x = (pre * (v0.x - v1.x) - (input.position.x - rayEnd.x) * post) / d;
			f32 y = (pre * (v0.y - v1.y) - (input.position.y - rayEnd.y) * post) / d;

			// Check if point is on lines
			if (x < Math::Min(input.position.x, rayEnd.x) || x > Math::Max(input.position.x, rayEnd.x) || y < Math::Min(input.position.y, rayEnd.y) || y > Math::Max(input.position.y, rayEnd.y))
				continue;
			if (x < Math::Min(v0.x, v1.x) || x > Math::Max(v0.x, v1.x) || y < Math::Min(v0.y, v1.y) || y > Math::Max(v0.y, v1.y))
				continue;

			f32v2 point(x, y);
			f32 l = (input.position - point).Length();

			if (l > input.length)
				continue;

			f32 fraction = l / input.length;
			if (!output.hit || output.fraction < fraction)
			{
				output.hit = true;
				output.pShape = pShape;
				output.normal = pShape->m_Normals[i];
				output.fraction = fraction;
			}
		}
	}
}
