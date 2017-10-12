#pragma once
#include "Common/BaseTypes.h"
#include "Common/Settings.h"
#include "Vec2.h"

namespace P2D {
	
	struct Transform
	{
		f32v2 position;
		f32 rotation;

		Transform();

		f32v2 Move(const f32v2& relpos) const;
	};

}
#include "Transform.inl"