#pragma once

#include <memory>
#include <functional>
#include <iostream>

namespace ModernChess
{
    /**
     * @brief Allocates aligned memory for better performance
     */
    class MemoryAllocator
    {
    public:
        MemoryAllocator() = delete;

        template<typename T>
        static std::unique_ptr<T, std::function<void(T*)>> alignedAlloc(size_t allocSizeBytes)
        {
            T* ptr = static_cast<T*>(MemoryAllocator::alignedAllocRawPtr(allocSizeBytes));

            if (not ptr)
            {
                std::cerr << "Failed to allocate " << allocSizeBytes << " bytes!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::function<void(T*)> deleter = [](T *ptr) {
                MemoryAllocator::alignedFree((void*) ptr);
            };

            return std::unique_ptr<T, std::function<void(T*)>>(ptr, deleter);
        }

        template<typename T>
        static std::unique_ptr<T[], std::function<void(T*)>> alignedArray(size_t allocSizeBytes)
        {
            T* ptr = static_cast<T*>(MemoryAllocator::alignedAllocRawPtr(allocSizeBytes));

            if (not ptr)
            {
                std::cerr << "Failed to allocate " << allocSizeBytes << " bytes!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::function<void(T*)> deleter = [](T *ptr) {
                MemoryAllocator::alignedFree((void*) ptr);
            };

            return std::unique_ptr<T[], std::function<void(T*)>>(ptr, deleter);
        }
    private:
        static void* alignedAllocRawPtr(size_t allocSizeBytes);

        static void alignedFree(void* ptr);
    };
}