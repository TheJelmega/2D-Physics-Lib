#include "Physics/Material.h"

namespace P2D {


	Material::Material()
		: restitution(0.f)
		, staticFriction(0.1f)
		, dynamicFriction(0.1f)
		, density(1.f)
	{
	}
}
