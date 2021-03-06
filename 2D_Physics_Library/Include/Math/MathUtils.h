#pragma once
#include "../Common/BaseTypes.h"
#include <cmath>


namespace P2D { namespace Math {
	
	namespace Detail {
		constexpr f64 Epsilon = 0.0001;
		constexpr f64 Pi = 3.1415926535897932;
	}

	template<typename T>
	constexpr T Epsilon = T(Detail::Epsilon);
	template<typename T>
	constexpr T Pi = T(Detail::Pi);

	template<typename T>
	T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	/**
	 * Check if 2 values are equal
	 */
	template<typename T>
	bool Equals(T a, T b)
	{
		return abs(a - b) < Epsilon<T>;
	}

	/**
	* Check if 2 values are equal with an epsilon
	*/
	template<typename T>
	bool Equals(T a, T b, T epsilon)
	{
		return abs(a - b) < epsilon;
	}

	template<typename T>
	T Clamp(T val, T min, T max)
	{
		if (val < min)
			return min;
		if (val > max)
			return max;
		return val;
	}

	template<typename T>
	T ToRadians(T degrees)
	{
		return degrees * Pi<f32> / T(180.f);
	}

} }
