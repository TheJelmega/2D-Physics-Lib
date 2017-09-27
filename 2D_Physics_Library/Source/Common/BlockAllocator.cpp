#include "Common/BlockAllocator.h"
#include "Common/Settings.h"

namespace P2D {

	bool BlockAllocator::m_IsLookupInitialzed = false;

	// Needed to compile, since m_BlockLookup is constexpr: m_BlockLookup 'declaration'
	u8 BlockAllocator::m_BlockLookup[];

	BlockAllocator::BlockAllocator()
	{
		m_pData = static_cast<u8*>(P2D_ALLOCATE(m_ChunkSize + sizeof(ChunkHeader)));
		ChunkHeader* header = reinterpret_cast<ChunkHeader*>(m_pData);
		header->next = nullptr;
		header->index = 0;

		memset(m_FreeLists, 0, m_NumBlockSizes * sizeof(BlockHeader));

		if (!m_IsLookupInitialzed)
		{
			u32 j = 0;
			for (u32 i = 0; i < m_LookupSize; ++i)
			{
				if ((i + 1) * 16 <= m_BlockSizes[j])
				{
					m_BlockLookup[i] = j;
				}
				else
				{
					++j;
					m_BlockLookup[i] = j;
				}
			}
			m_IsLookupInitialzed = true;
		}
	}

	BlockAllocator::~BlockAllocator()
	{
		ChunkHeader* pChunk = reinterpret_cast<ChunkHeader*>(m_pData - sizeof(ChunkHeader));
		ChunkHeader* pNext;
		while (pChunk)
		{
			pNext = pChunk->next;
			P2D_FREE(pChunk);
			pChunk = pNext;
		}
	}

	void* BlockAllocator::Allocate(size_t size)
	{
		if (size == 0)
			return nullptr;

		if (size > m_MaxBlockSize)
			return P2D_ALLOCATE(size);

		u8 index = m_BlockLookup[size / 16];
		P2D_ASSERT(index < m_NumBlockSizes);

		if (m_FreeLists[index])
		{
			BlockHeader* block = m_FreeLists[index];
			m_FreeLists[index] = block->next;
			return block;
		}
		else
		{
			ChunkHeader* pChunk = reinterpret_cast<ChunkHeader*>(m_pData);
			u32 blockSize = m_BlockSizes[index];
			while (true)
			{
				if (m_ChunkSize - pChunk->index > blockSize)
				{
					u8* data = m_pData + sizeof(ChunkHeader) + pChunk->index;
					pChunk->index += blockSize;
					return data;
				}
				ChunkHeader* pNext = pChunk->next;
				if (!pNext)
					break;
				pChunk = pNext;
			}

			ChunkHeader* pNewChunk = (ChunkHeader*)P2D_ALLOCATE(m_ChunkSize + sizeof(ChunkHeader));
			pNewChunk->next = nullptr;
			pNewChunk->index = blockSize;
			pChunk->next = pNewChunk;
			return m_pData + pNewChunk->index + sizeof(ChunkHeader);

		}

		return nullptr;
	}

	void BlockAllocator::Deallocate(void* ptr, size_t size)
	{
		if (size == 0)
			return;

		if (size > m_MaxBlockSize)
		{
			P2D_FREE(ptr);
			return;
		}

		u8 index = m_BlockLookup[size / 16];
		P2D_ASSERT(index < m_NumBlockSizes);

		BlockHeader* pBlock = static_cast<BlockHeader*>(ptr);
		pBlock->next = nullptr;
		BlockHeader* pFree = m_FreeLists[index];

		if (m_FreeLists[index])
		{
			BlockHeader* pNext = pFree->next;
			while (pNext)
			{
				pFree = pNext;
				pNext = pFree;
			}
			pFree->next = pBlock;
		}
		else
		{
			m_FreeLists[index] = pBlock;
		}
	}
}
