#pragma once
#include "Transform.h"

namespace P2D {
	
	P2D_INL Transform::Transform() : rotation(0)
	{
	}

	P2D_INL f32v2 Transform::Move(const f32v2& relpos) const
	{
		return position + relpos.Rotated(rotation);
	}

	P2D_INL Velocity::Velocity() : angularVelocity(0)
	{
	}

}
