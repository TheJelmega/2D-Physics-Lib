#include "Common/StackAllocator.h"
#include "Common/Settings.h"
#include <memory>

namespace P2D {


	StackAllocator::Entry::Entry()
		: size(0)
		, pData(nullptr)
		, usedMalloc(false)
	{
	}

	StackAllocator::StackAllocator()
	{
		m_pData = static_cast<u8*>(P2D_ALLOCATE(m_ReservedSize));
	}

	StackAllocator::~StackAllocator()
	{
		Reset();
		P2D_FREE(m_pData);
	}

	void* StackAllocator::Allocate(size_t size)
	{
		P2D_ASSERT(m_CurrentEntry != m_MaxEntries);

		if (size == 0)
			return nullptr;

		Entry* pEntry = m_Entries + m_CurrentEntry;
		pEntry->size = size;

		if (size > m_ReservedSize - m_Index)
		{
			void* data = P2D_ALLOCATE(size);
			pEntry->pData = data;
			pEntry->usedMalloc = true;
		}
		else
		{
			void* data = m_pData + m_Index;
			m_Index += size;

			pEntry->pData = data;
			pEntry->usedMalloc = false;
		}
		++m_CurrentEntry;

		return m_Entries[m_CurrentEntry - 1].pData;
	}

	void StackAllocator::Deallocate(void* ptr)
	{
		P2D_ASSERT(ptr);
		--m_CurrentEntry;
		Entry* entry = m_Entries + m_CurrentEntry;
		P2D_ASSERT(ptr == entry->pData);

		if (entry->usedMalloc)
		{
			P2D_FREE(entry->pData);
		}
		else
		{
			m_Index -= entry->size;
		}
	}

	void StackAllocator::Reset()
	{
		--m_CurrentEntry;
		for (i32 i = 0; i > 0; --i)
		{
			Deallocate(m_Entries[i].pData);
		}
	}
}
