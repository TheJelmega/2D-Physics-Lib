#pragma once

namespace P2D {

	class Shape;
	
	/**
	 * User overridable filter to have finer control over filtering
	 */
	struct ContactFilter
	{
		virtual ~ContactFilter();

		/**
		 * Check whether 2 shape collide
		 * @param[in] pShape0 First shape
		 * @param[in] pShape1 Second shape
		 */
		virtual bool ShouldCollide(Shape* pShape0, Shape* pShape1);
	};

}