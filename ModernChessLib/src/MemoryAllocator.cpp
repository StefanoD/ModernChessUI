#include "ModernChess/MemoryAllocator.h"

namespace ModernChess
{
    void* std_aligned_alloc(size_t alignment, size_t size)
    {
#if defined(POSIXALIGNEDALLOC)
        void *mem;
        return posix_memalign(&mem, alignment, size) ? nullptr : mem;
#elif defined(_WIN32)
        return _mm_malloc(size, alignment);
#else
        return std::aligned_alloc(alignment, size);
#endif
    }

    void* MemoryAllocator::alignedAllocRawPtr(size_t allocSizeBytes)
    {
#if defined(__linux__)
        constexpr size_t alignment = 2UL * 1024 * 1024; // assumed 2MB page size
#elif defined(__APPLE__)
        constexpr size_t alignment = 16UL * 1024;
#else
        constexpr size_t alignment = 4UL * 1024; // assumed small page size
#endif
        // round up to multiples of alignment
        const size_t size = ((allocSizeBytes + alignment - 1) / alignment) * alignment;
        void *mem = std_aligned_alloc(alignment, size);
#if defined(MADV_HUGEPAGE)
        madvise(mem, size, MADV_HUGEPAGE);
#endif
        return mem;
    }

    void MemoryAllocator::alignedFree(void* ptr)
    {
#if defined(POSIXALIGNEDALLOC)
        free(ptr);
#elif defined(_WIN32)
        _mm_free(ptr);
#else
        std::free(ptr);
#endif
    }
}