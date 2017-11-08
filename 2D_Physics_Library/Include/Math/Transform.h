#pragma once
#include "Common/BaseTypes.h"
#include "Common/Settings.h"
#include "Vec2.h"

namespace P2D {
	
	struct Transform
	{
		f32v2 position;	/**< Position*/
		f32 rotation;	/**< Rotation/angle*/

		Transform();

		/**
		 * Get the global position from a relative position
		 * @param[in] relpos	Relative position
		 */
		f32v2 Move(const f32v2& relpos) const;
	};

	struct Velocity
	{
		f32v2 linearVelocity;	/**< Linear velocity*/
		f32 angularVelocity;	/**< Angular velocity*/

		Velocity();
	};

}
#include "Transform.inl"