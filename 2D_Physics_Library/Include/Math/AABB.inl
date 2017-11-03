#pragma once
#include "AABB.h"
#include "Common/Settings.h"

namespace P2D {


	P2D_INL AABB::AABB()
	{
	}

	P2D_INL AABB::AABB(const f32v2& min, const f32v2& max) : min(min), max(max)
	{
	}

	P2D_INL AABB::AABB(f32 left, f32 bottom, f32 right, f32 top) : min(left, bottom), max(right, top)
	{
	}

	P2D_INL AABB::AABB(const AABB& aabb) : min(aabb.min), max(aabb.max)
	{
	}

	P2D_INL AABB::AABB(AABB&& aabb) noexcept : min(aabb.min), max(aabb.max)
	{
	}

	P2D_INL AABB& AABB::operator=(const AABB& aabb)
	{
		min = aabb.min;
		max = aabb.max;
		return *this;
	}

	P2D_INL AABB& AABB::operator=(AABB&& aabb) noexcept
	{
		min = aabb.min;
		max = aabb.max;
		return *this;
	}

	P2D_INL void AABB::Move(const f32v2& v)
	{
		min += v;
		max += v;
	}

	P2D_INL void AABB::Combine(const AABB& aabb)
	{
		min.x = Math::Min(min.x, aabb.min.x);
		min.y = Math::Min(min.y, aabb.min.y);
		max.x = Math::Max(max.x, aabb.max.x);
		max.y = Math::Max(max.y, aabb.max.y);
	}

	P2D_INL bool AABB::Overlaps(const AABB& aabb) const
	{
		return min <= aabb.max && max >= aabb.min;
	}

	P2D_INL bool AABB::Contains(const AABB& aabb) const
	{
		return aabb.min >= min && aabb.max <= max;
	}

	P2D_INL f32 AABB::GetPerimeter() const
	{
		f32v2 diff = max - min;
		return 2.f * (diff.x + diff.y);
	}

	P2D_INL void AABB::Pad(f32 value)
	{
		min -= value;
		max += value;
	}
}
