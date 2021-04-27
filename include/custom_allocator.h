#pragma once

/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <memory>
#include <vector>
#include <cassert>

namespace neko
{

    //#define NEKO_ALLOCATOR_LOG

    class Allocator
    {
    public:

        Allocator() = delete;
        Allocator(size_t size, void* start);
        Allocator(const Allocator&);
        Allocator(Allocator&&) = default;
        Allocator& operator=(const Allocator&) = delete;
        Allocator& operator=(Allocator&&) = default;

        virtual ~Allocator() noexcept;

        virtual void* Allocate(size_t allocatedSize, size_t alignment) = 0;

        virtual void Deallocate(void* p) = 0;

        inline static size_t CalculateAlignForwardAdjustment(const void* address, size_t alignment)
        {
            //Check if alignement is power of 2
            assert((alignment & (alignment - 1)) == 0);
            const size_t adjustment = alignment - ((std::size_t) const_cast<void*>(address) & ((alignment - 1)));

            if (adjustment == alignment) return 0;
            return adjustment;
        }

        inline static size_t
            CalculateAlignForwardAdjustmentWithHeader(const void* address, size_t alignment, size_t headerSize)
        {
            auto adjustment = CalculateAlignForwardAdjustment(address, alignment);
            size_t neededSpace = headerSize;
            if (adjustment < neededSpace)
            {
                neededSpace -= adjustment;
                adjustment += alignment * (neededSpace / alignment);
                if (neededSpace % alignment > 0) adjustment += alignment;
            }
            return adjustment;
        }

        inline static void* AlignForward(void* address, size_t alignment)
        {
            return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) + CalculateAlignForwardAdjustment(address, alignment));
        }

        inline static void* AlignForwardWithHeader(void* address, size_t alignment, size_t headerSize)
        {
            return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(address) +
                CalculateAlignForwardAdjustmentWithHeader(address, alignment, headerSize));
        }

        [[nodiscard]] size_t GetUsedMemory() const
        {
            return usedMemory_;
        }

        [[nodiscard]] void* GetStart() const
        {
            return start_;
        }

        [[nodiscard]] size_t GetSize() const
        {
            return size_;
        }

        virtual void Destroy();


    protected:
        void* start_ = nullptr;
        size_t size_ = 0;
        size_t usedMemory_ = 0;
        size_t numAllocations_ = 0;
    };

    class LinearAllocator : public Allocator
    {
    public:
        LinearAllocator(size_t size, void* start) : Allocator(size, start), currentPos_(start_)
        {
            assert(size > 0);
        }

        ~LinearAllocator() override
        {
            currentPos_ = nullptr;
        }

        LinearAllocator(const LinearAllocator&) = delete;

        LinearAllocator& operator=(const LinearAllocator&) = delete;

        void* Allocate(size_t allocatedSize, size_t alignment) override;

        void Deallocate(void* p) override;

        void Clear();

    protected:
        void* currentPos_ = nullptr;
    };
}
