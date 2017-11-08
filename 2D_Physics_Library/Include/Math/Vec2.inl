#pragma once
#include "Vec2.h"
#include "MathUtils.h"

namespace P2D {
	
	template <typename T>
	Vec2<T>::Vec2() : x(0), y(0)
	{
	}

	template <typename T>
	template<typename U>
	Vec2<T>::Vec2(U val) : x(T(val)), y(T(val))
	{
	}

	template <typename T>
	template <typename X, typename Y>
	Vec2<T>::Vec2(X x, Y y) : x(T(x)), y(T(y))
	{
	}

	template <typename T>
	template <typename U>
	Vec2<T>::Vec2(const Vec2<U>& v) : x(T(v.x)), y(T(v.y))
	{
	}

	template <typename T>
	Vec2<T>::Vec2(const Vec2& v) : x(v.x), y(v.y)
	{
	}

	template <typename T>
	Vec2<T>::Vec2(Vec2&& v) noexcept : x(v.x), y(v.y)
	{
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator+() const
	{
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator-() const
	{
		return Vec2(-x, -y);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator+(const Vec2& v) const
	{
		return Vec2(x + v.x, y + v.y);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator-(const Vec2& v) const
	{
		return Vec2(x - v.x, y - v.y);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator*(const Vec2& v) const
	{
		return Vec2(x * v.x, y * v.y);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator/(const Vec2& v) const
	{
		return Vec2(x / v.x, y / v.y);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator+(T val) const
	{
		return Vec2(x + val, y + val);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator-(T val) const
	{
		return Vec2(x - val, y - val);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator*(T val) const
	{
		return Vec2(x * val, y * val);
	}

	template <typename T>
	Vec2<T> Vec2<T>::operator/(T val) const
	{
		return Vec2(x / val, y / val);
	}

	template <typename T>
	template <typename U>
	Vec2<T>& Vec2<T>::operator=(const Vec2<U>& v)
	{
		x = T(v.x);
		y = T(v.y);
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator=(Vec2&& v) noexcept
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator*=(const Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator/=(const Vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator+=(T val)
	{
		x += val;
		y += val;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator-=(T val)
	{
		x -= val;
		y -= val;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator*=(T val)
	{
		x *= val;
		y *= val;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::operator/=(T val)
	{
		x /= val;
		y /= val;
		return *this;
	}

	template <typename T>
	T Vec2<T>::Length() const
	{
		return sqrt(x * x + y * y);
	}

	template <typename T>
	T Vec2<T>::SqLength() const
	{
		return x * x + y * y;
	}

	template <typename T>
	T Vec2<T>::Distance(const Vec2& v)
	{
		return (v - *this).Length();
	}

	template <typename T>
	T Vec2<T>::SqDistance(const Vec2& v)
	{
		return (v - *this).SqLength();
	}

	template <typename T>
	T Vec2<T>::Dot(const Vec2& v) const
	{
		return x * v.x + y * v.y;
	}

	template <typename T>
	T Vec2<T>::Cross(const Vec2& v) const
	{
		return x * v.y - y * v.x;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Cross(T val) const
	{
		return Vec2(-y * val, x * val);
	}

	template <typename T>
	Vec2<T> Vec2<T>::Normalized() const
	{
		T l = SqLength();
		if (l < Math::Epsilon<T> || Math::Equals(l, T(1)))
			return *this;
		return *this * (T(1) / sqrt(l));
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Normalize()
	{
		T l = SqLength();
		if (l < Math::Epsilon<T> || Math::Equals(l, T(1)))
			return *this;
		*this *= (T(1) / sqrt(l));
		return *this;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Lerp(const Vec2& v, T factor) const
	{
		return *this + (v - *this) * factor;
	}

	template <typename T>
	Vec2<T> Vec2<T>::Rotated(T angle) const
	{
		T s = T(sin(angle));
		T c = T(cos(angle));
		return Vec2(x * c - y * s, x * s + y * c);
	}

	template <typename T>
	Vec2<T>& Vec2<T>::Rotate(T angle)
	{
		T s = T(sin(angle));
		T c = T(cos(angle));
		T tx = x * c - y * s;
		T ty = x * s + y * c;
		x = tx;
		y = ty;
		return *this;
	}

	template <typename T>
	Vec2<T>& Vec2<T>::RotateAroundPoint(const Vec2& point, T angle)
	{
		Vec2 pos = *this - point;
		pos.Rotate(angle);
		*this = pos + point;
		return *this;
	}

	template <typename T>
	T Vec2<T>::Angle() const
	{
		return atan2(y, x);
	}

	template <typename T>
	T Vec2<T>::Angle(const Vec2& v) const
	{
		Vec2 v1 = Normalized();
		Vec2 v2 = v.Normalized();
		T dot = v1.Dot(v2);
		dot = Math::Clamp(dot, -1.f, 1.f);
		T angle;
		if (dot == 0)
			angle = Math::Pi<f32> / 2.f;
		else
			angle = acos(dot);
		T cross = v1.Cross(v2);
		if (cross >= 0)
			return angle;
		return -angle;
	}

	template <typename T>
	bool Vec2<T>::operator==(const Vec2& v) const
	{
		return x == v.x && y == v.y;
	}

	template <typename T>
	bool Vec2<T>::operator!=(const Vec2& v) const
	{
		return x != v.x || y != v.y;
	}

	template <typename T>
	bool Vec2<T>::operator<(const Vec2& v) const
	{
		return x < v.x && y < v.y;
	}

	template <typename T>
	bool Vec2<T>::operator<=(const Vec2& v) const
	{
		return x <= v.x && y <= v.y;
	}

	template <typename T>
	bool Vec2<T>::operator>(const Vec2& v) const
	{
		return x > v.x && y > v.y;
	}

	template <typename T>
	bool Vec2<T>::operator>=(const Vec2& v) const
	{
		return x >= v.x && y >= v.y;
	}

	template <typename T>
	bool Vec2<T>::Equals(const Vec2& v) const
	{
		return Math::Equals(x, v.x) && Math::Equals(y, v.y);
	}

	template <typename T>
	bool Vec2<T>::Equals(const Vec2& v, T epsilon) const
	{
		return Math::Equals(x, v.x, epsilon) && Math::Equals(y, v.y, epsilon);
	}

	template <typename T>
	Vec2<T> operator*(T val, const Vec2<T>& v)
	{
		return Vec2<T>(v.x * val, v.y * val);
	}

	// Constants
	template<typename T>
	const Vec2<T> Vec2<T>::Zero = Vec2<T>(0, 0);
	template<typename T>
	const Vec2<T> Vec2<T>::One = Vec2<T>(1, 1);
	template<typename T>
	const Vec2<T> Vec2<T>::AxisX = Vec2<T>(1, 0);
	template<typename T>
	const Vec2<T> Vec2<T>::AxisY = Vec2<T>(0, 1);
	template<typename T>
	const Vec2<T> Vec2<T>::Left = Vec2<T>(-1, 0);
	template<typename T>
	const Vec2<T> Vec2<T>::Right = Vec2<T>(1, 0);
	template<typename T>
	const Vec2<T> Vec2<T>::Up = Vec2<T>(0, 1);
	template<typename T>
	const Vec2<T> Vec2<T>::Down = Vec2<T>(0, -1);
}
