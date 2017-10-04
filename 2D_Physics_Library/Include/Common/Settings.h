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
	

	constexpr f32 g_MaxDeltaTime = 1.f / 30.f;

}