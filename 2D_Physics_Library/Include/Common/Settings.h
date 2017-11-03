// Configuration file
#pragma once
#include <cassert>
#include <memory>
#include "Common/BaseTypes.h"

#if defined(DEBUG) || defined(_DEBUG)
#	define P2D_DEBUG
#endif

#ifdef P2D_DEBUG
#	define P2D_ASSERT(x) assert(x)
#else
#	define P2D_ASSERT(x)
#endif

#define P2D_INL inline
#ifdef _MSC_VER
#	define P2D_FORCE_INL __forceinline // MSVC specific force inline
#else
#	define P2D_FORCE_INL inline __attribute__((always_inline)) // general force inline (non-MSVC)
#endif


// Macros can be used to change with external allocator
#ifndef P2D_ALLOCATE
#	define P2D_ALLOCATE(size) malloc(size)
#endif
#ifndef P2D_FREE
#	define P2D_FREE(ptr) free(ptr)
#endif

// Settings
namespace P2D {
	
	/**Maximum amount of time to update after program has paused*/
	constexpr f32 g_MaxDeltaTime = 1.f / 30.f;

	/**Maximum amount of mnaifold points*/
	constexpr u32 g_MaxManifoldPairs = 4;

	/**Mass of a static object*/
	constexpr f32 g_StaticMass = (std::numeric_limits<f32>::max)();
	/**Inertia of static object*/
	constexpr f32 g_StaticInertia = (std::numeric_limits<f32>::max)();

	/**Small length used as collision and constraint tollerance*/
	constexpr f32 g_LinearSlop = 0.005f;

	/**Skin whtickness of polyogns/edges*/
	constexpr f32 g_SkinThickness = 2.f * g_LinearSlop;

	/**Value to fatten AABB with*/
	constexpr f32 g_AABBExtension = 0.1f;
	/**Used for AABBTree's move predicition*/
	constexpr f32 g_AABBMultiplier = 2.f;

	/**Max linear velocity of an object*/
	constexpr f32 g_MaxLinearVelocity = 10'000.f;
	/**Max angular velocity of an object*/
	constexpr f32 g_MaxAngulareVelocity = 10'000.f;

	/**Max sleep linear velocity*/
	constexpr f32 g_MaxSleepLinearVelocity = 0.2f;
	/**Max sleep angular velocity*/
	constexpr f32 g_MaxSleepAngularVelocity = 0.2f;
	/**Time to sleep*/
	constexpr f32 g_TimeToSleep = 0.5f;
}