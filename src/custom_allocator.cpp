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

#include "custom_allocator.h"


namespace neko
{
Allocator::Allocator(size_t size, void* start) : start_(start), size_(size)
{
}

Allocator::Allocator(const Allocator& allocator)
{

    start_ = allocator.start_;
    size_ = allocator.size_;
    numAllocations_ = allocator.numAllocations_;
    usedMemory_ = allocator.usedMemory_;
}

Allocator::~Allocator() noexcept
{

    assert(numAllocations_ == 0 && usedMemory_ == 0);
}

void Allocator::Destroy()
{
    assert(numAllocations_ == 0 && usedMemory_ == 0);
    start_ = nullptr;
    size_ = 0;
}

void* LinearAllocator::Allocate(size_t allocatedSize, size_t alignment)
{
    assert(allocatedSize != 0);
    const auto adjustment = CalculateAlignForwardAdjustment(currentPos_, alignment);

    assert(usedMemory_ + adjustment + allocatedSize < size_);

    auto* alignedAddress = (void*)((std::uint64_t) currentPos_ + adjustment);
    currentPos_ = (void*)((std::uint64_t) alignedAddress + allocatedSize);
    usedMemory_ += allocatedSize + adjustment;
    numAllocations_++;
    return alignedAddress;
}

void LinearAllocator::Deallocate([[maybe_unused]] void* p)
{
    assert(false);
}

void LinearAllocator::Clear()
{
    numAllocations_ = 0;
    usedMemory_ = 0;
    currentPos_ = start_;
}

}
