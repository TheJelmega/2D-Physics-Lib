// Configuration file
#pragma once
#include <cassert>
#include <memory>

#if defined(DEBUG) || defined(_DEBUG)
#	define P2D_DEBUG
#endif

#ifdef P2D_DEBUG
#	define P2D_ASSERT(x) assert(x)
#else
#	define P2D_ASSERT(x)
#endif

// Macros can be used to change with external allocator
#define P2D_ALLOCATE(size) malloc(size)
#define P2D_FREE(ptr) free(ptr)