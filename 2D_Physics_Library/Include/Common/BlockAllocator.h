#pragma once
#include "Common/BaseTypes.h"

namespace P2D {
	
	class BlockAllocator
	{
	private:
		struct BlockHeader
		{
			BlockHeader* next;
		};
		struct ChunkHeader
		{
			ChunkHeader* next;
			u32 index;
		};
	public:
		BlockAllocator();
		~BlockAllocator();

		void* Allocate(size_t size);
		void Deallocate(void* ptr, size_t size);

	private:

		static constexpr u32 m_ChunkSize = 128 * 1024; //128K
		static constexpr u32 m_NumBlockSizes = 16;
		static constexpr u32 m_MaxBlockSize = 512;
		static constexpr u32 m_BlockSizes[m_NumBlockSizes] =
		{
			16, 32, 48, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 448, 512
		};
		static constexpr u32 m_LookupSize = m_MaxBlockSize / 16;
		static u8 m_BlockLookup[m_LookupSize];
		static bool m_IsLookupInitialzed;


		u8* m_pData;
		BlockHeader* m_FreeLists[m_NumBlockSizes];

	};

}