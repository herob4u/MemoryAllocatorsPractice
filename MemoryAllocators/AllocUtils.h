#pragma once

#include <stdint.h>
#include <intrin.h>

#include <assert.h>

namespace AllocUtils
{
    inline bool IsPow2(uint32_t val)
    {
        return (val & (val - 1)) == 0;
    }
    
    inline bool IsPow2(uint64_t val)
    {
        return (val & (val - 1)) == 0;
    }

    uint32_t NextPow2(uint32_t val)
    {
        --val;
        val = val | (val >> 1);
        val = val | (val >> 2);
        val = val | (val >> 4);
        val = val | (val >> 8);
        val = val | (val >> 16);
        ++val;

        return val;
    }

    uint32_t NextPow2Fast(uint32_t val)
    {
        if(val == 0)
        {
            return 1;
        }

        // Is a power of 2? Return itself
        if(IsPow2(val))
        {
            return val;
        }

        return (1 << (32 -__lzcnt(val)));
    }

    /* Given a size, return the required allocation size to fit the desired power of 2 alignment.
    *  ex: val = 2, alignment = 16 --> Need at least 16 bytes (next multiple of 16)
    *  ex: val = 33, alignment = 16 --> Need at least 48 bytes
    *  ex: val = 33, alignment = 8 --> Need at least 40 bytes (next multiple of 8)
    */
    inline size_t AlignPow2(size_t val, size_t alignment)
    {
        assert(IsPow2(alignment));
        return (val + alignment - 1) & ~(alignment - 1);
    }

    inline size_t Align8(size_t val)
    {
        return AlignPow2(val, 8);
    }

    inline size_t Align16(size_t val)
    {
        return AlignPow2(val, 16);
    }
}