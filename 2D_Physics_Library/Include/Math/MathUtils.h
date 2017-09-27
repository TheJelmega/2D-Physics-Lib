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

} }
