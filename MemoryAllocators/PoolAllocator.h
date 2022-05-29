#pragma once
#include <cstdlib>
#include <assert.h>
#include <sstream>

#define REUSE_CHUNKS

struct Chunk
{
    Chunk* m_next;
};

/* This pool allocator works by virtue of FIXED size chunks.
*  The primary disadvantage is internal fragmentation in elements have varying sizes, and never fully utilize a chunk
*  This can be easily alleviated by using the allocate size argument as the chunk size, and then the block size adjusts accordingly - i.e block size = requestedBytes * ChunksPerBlock
*/
template<size_t ChunkSizeBytes = 1024, size_t ChunksPerBlock = 8>
class PoolAllocator
{
public:
    PoolAllocator(){}

    void* Allocate(size_t bytes)
    {
        assert(bytes <= ChunkSizeBytes);
        if(bytes > ChunkSizeBytes) { return nullptr; }

        // Allocate a new block
        if(!m_allocPtr)
        {
            m_allocPtr = AllocateBlock();
        }


        void* p = m_allocPtr;
        m_allocPtr = m_allocPtr->m_next;
        return p;
    }

    void Deallocate(void* p)
    {
        assert(p);

#ifndef REUSE_CHUNKS
        free(p);
#else
        // Reuse the chunk by appending it back to the front of the list.
        reinterpret_cast<Chunk*>(p)->m_next = m_allocPtr;
        m_allocPtr = reinterpret_cast<Chunk*>(p);
#endif // !REUSE_CHUNKS

        
    }

    // Spits out the remaining chunks
    std::string GetMemoryDump()
    {
        std::stringstream ss;
        char* it = (char*)m_allocPtr;
        int num = 0;
        while(it)
        {
            ss << "Chunk " << num++ << ": " << (void*)it << "\n";

            if(((Chunk*)it)->m_next == nullptr)
            {
                break;
            }

            it = (char*)m_allocPtr + (num * ChunkSizeBytes);
        }

        return ss.str();
    }

private:
    Chunk* AllocateBlock()
    {
        char* block = (char*)malloc(ChunksPerBlock * ChunkSizeBytes);
        for(int i = 0; i < ChunksPerBlock - 1; ++i)
        {
            ((Chunk*)(block + (i * ChunkSizeBytes)))->m_next = reinterpret_cast<Chunk*>(block + ((i+1) * ChunkSizeBytes));
        }

        ((Chunk*)(block + ((ChunksPerBlock - 1) * ChunkSizeBytes)))->m_next = nullptr;
        return (Chunk*)block;
    }

    Chunk* m_allocPtr;
};