#pragma once
#include "BaseTypes.h"

namespace P2D {
	
	class StackAllocator
	{
		struct Entry
		{
			size_t size;
			void* pData;
			bool usedMalloc;

			Entry();
		};

	public:
		StackAllocator();
		~StackAllocator();

		/**
		 * Allocate memory
		 * @param[in] size	Size of allocation
		 */
		void* Allocate(size_t size);
		/**
		 * Deallocate memory
		 * @note Deallocation needs to happen in the same order as allocation
		 */
		void Deallocate(void* ptr);

		/**
		 * Reset the allocator to it's begin state
		 */
		void Reset();

	private:

		// Max amount of allocator entries
		static constexpr size_t m_MaxEntries = 64;
		// Allocated size for memory buffer
		static constexpr size_t m_ReservedSize = 128 * 1024; // 128k

		u8* m_pData;
		u32 m_Index = 0;

		Entry m_Entries[m_MaxEntries];
		u16 m_CurrentEntry = 0;
	};

}
