#pragma once
#include "Common/BaseTypes.h"

namespace P2D {
	
	/**
	 * Material
	 */
	struct Material
	{
		f32 restitution;
		f32 staticFriction;
		f32 dynamicFriction;
		f32 density;

		Material();
	};

}