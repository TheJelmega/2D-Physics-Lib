#pragma once
#include "Common/BaseTypes.h"

namespace P2D {
	
	struct Material
	{
		f32 restitution;		/**< Restitution/bounciness*/
		f32 staticFriction;		/**< static friction*/
		f32 dynamicFriction;	/**< dynamic friction*/
		f32 density;			/**< density*/

		Material();
	};

}