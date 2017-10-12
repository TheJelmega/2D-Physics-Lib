#pragma once
#include <type_traits>

namespace P2D {
	
	template<typename T>
	struct Vec2
	{
		static_assert(std::is_arithmetic<T>::value, "T needs to be an arithmetic type");

		T x, y;

		/**
		 * Create a vec2
		 */
		Vec2();
		/**
		* Create a vec2
		* @param[in] val	Value
		*/
		template<typename U>
		explicit Vec2(U val);
		/**
		 * Create a vec2
		 * @param[in] x	X-value
		 * @param[in] y	Y-value
		 */
		template<typename X, typename Y>
		Vec2(X x, Y y);
		/**
		 * Create a vec2 from another vec2
		 * @param[in] v	Vec2
		 */
		template<typename U>
		Vec2(const Vec2<U>& v);
		Vec2(const Vec2& v);
		Vec2(Vec2&& v) noexcept;

		Vec2 operator+() const;
		Vec2 operator-() const;
		Vec2 operator+(const Vec2& v) const;
		Vec2 operator-(const Vec2& v) const;
		Vec2 operator*(const Vec2& v) const;
		Vec2 operator/(const Vec2& v) const;
		Vec2 operator+(T val) const;
		Vec2 operator-(T val) const;
		Vec2 operator*(T val) const;
		Vec2 operator/(T val) const;

		template<typename U>
		Vec2& operator=(const Vec2<U>& v);
		Vec2& operator=(const Vec2& v);
		Vec2& operator=(Vec2&& v) noexcept;

		Vec2& operator+=(const Vec2& v);
		Vec2& operator-=(const Vec2& v);
		Vec2& operator*=(const Vec2& v);
		Vec2& operator/=(const Vec2& v);
		Vec2& operator+=(T val);
		Vec2& operator-=(T val);
		Vec2& operator*=(T val);
		Vec2& operator/=(T val);

		/**
		 * Get the length of the vector
		 */
		T Length() const;
		/**
		 * Get the square length of the vector
		 */
		T SqLength() const;

		/**
		 * Get the distance between 2 vectors
		 */
		T Distance(const Vec2& v);
		/**
		 * Get the square distance between 2 vectors
		 */
		T SqDistance(const Vec2& v);

		/**
		 * Get the dot product of 2 vectors
		 * @param[in] v	Vec2
		 */
		T Dot(const Vec2& v) const;
		/**
		 * Get the 2D cross product of 2 vector
		 */
		T Cross(const Vec2& v) const;
		/**
		 * Get the 2D cross product of a vector and a scalar (Z-axis)
		 */
		Vec2 Cross(T val);

		/**
		 * Get the normalized version of the vector
		 */
		Vec2 Normalized() const;
		/**
		 * Normalize the vector
		 */
		Vec2& Normalize();

		/**
		 * Get the rotated vector
		 * @param[in] angle	Angle to rotate by
		 */
		Vec2 Rotated(T angle) const;
		/**
		* Rotate vector (changes vector)
		* @param[in] angle	Angle to rotate by
		*/
		Vec2& Rotate(T angle);
		
		/**
		 * Get the angle of a vector
		 */
		T Angle() const;
		/**
		 * Get the angle between 2 vectors
		 * @param[in] v	Vec2
		 */
		T Angle(const Vec2& v) const;

		/**
		 * Lerp between 2 vectors
		 */
		Vec2 Lerp(const Vec2& v, T factor);


		bool operator==(const Vec2& v) const;
		bool operator!=(const Vec2& v) const;
		bool operator<(const Vec2& v) const;
		bool operator<=(const Vec2& v) const;
		bool operator>(const Vec2& v) const;
		bool operator>=(const Vec2& v) const;
		/**
		 * Check if 2 vectors are the same
		 * @param[in] v	Vec2
		 */
		bool Equals(const Vec2& v) const;
		/**
		* Check if 2 vectors are the same, with an epsilon
		* @param[in] v			Vec2
		* @param[in] epsilon	Epsilon
		*/
		bool Equals(const Vec2& v, T epsilon) const;
		/**
		 * Check if a vector is in bounds
		 * @param[in] topLeft		Top left corner
		 * @param[in] bottomRight	Bottom Right corner
		 */
		bool IsInBounds(const Vec2& topLeft, const Vec2& bottomRight);
		/**
		* Check if a vector is in bounds
		* @param[in] left	Left bound
		* @param[in] top	Top bound
		* @param[in] right	Right bound
		* @param[in] bottom	Bottom bound
		*/
		bool IsInBounds(T left, T top, T right, T bottom);

		// Constants
		/** Zero vector */
		const static Vec2 Zero;
		/** One vector */
		const static Vec2 One;
		/** AxisX vector */
		const static Vec2 AxisX;
		/** AxisY vector */
		const static Vec2 AxisY;
		/** Left vector */
		const static Vec2 Left;
		/** Right vector */
		const static Vec2 Right;
		/** Up vector */
		const static Vec2 Up;
		/** Down vector */
		const static Vec2 Down;
	};

	template<typename T>
	Vec2<T> operator*(T val, const Vec2<T>& v);

}

typedef P2D::Vec2<f32> f32v2;
