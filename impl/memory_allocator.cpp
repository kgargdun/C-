
#include<iostream>
#include<cassert>
#include<cstdint>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

class MemoryAllocator {

private:
    struct alignas(std::max_align_t) BlockHeader {
        std::size_t size;
        bool isFree;
        BlockHeader *prev;
        BlockHeader *next;
    };

    void *arena;
    std::size_t arenaSize;
    BlockHeader *head;
    static constexpr std::size_t minBlockSize = sizeof(BlockHeader);


    std::size_t  getOffsetUp(std::size_t size) {
        std::size_t alignment = alignof(std::max_align_t);
        return (alignment - size%alignment) % alignment; 
    }

    std::size_t  getOffsetDown(std::size_t size) {
        std::size_t alignment = alignof(std::max_align_t);
        return size%alignment;
    }

    void mergeWithNext(BlockHeader *block) {
        if(!block->next) return;
        if(!block->next->isFree) return;
        block->size += block->next->size + sizeof(BlockHeader);
        block->next = block->next->next;
        if(block->next) block->next->prev = block;
    }

    void splitBlock(BlockHeader *block, std::size_t size) {
        
        if(size + sizeof(BlockHeader) >= block->size) return;
        BlockHeader *nextBlock = block->next;

        unsigned char *newBlockPtr = reinterpret_cast<unsigned char *>(block) + 
        (sizeof(BlockHeader) + size);

        BlockHeader *newBlock = new(newBlockPtr) BlockHeader;
        newBlock->prev = block;
        block->next = newBlock;
        newBlock->next = nextBlock; 
        newBlock->isFree = true;
        newBlock->size = block->size - size - sizeof(BlockHeader);
        block->size = size;
        if(nextBlock) nextBlock->prev = newBlock;
    }

    

public:

    MemoryAllocator(std::size_t arenaSize) : arenaSize(arenaSize) {
        assert(arenaSize > minBlockSize);
        arena = ::operator new(arenaSize);
        head = new(arena) BlockHeader();
        std::size_t remSize = arenaSize - sizeof(BlockHeader);

        std::size_t offset = getOffsetDown(remSize);
        remSize = remSize - offset;
        assert(remSize > 0);

        head->size = remSize;
        head->prev = nullptr;
        head->next = nullptr;
        head->isFree = true;

    }

    ~MemoryAllocator() {
        ::operator delete(arena);
    }

    void *allocate(std::size_t size) {
        BlockHeader *cur = head;
        std::size_t reqSize = size + getOffsetUp(size);

    
        while(cur) {
            if(cur->isFree && cur->size>=reqSize) {
                splitBlock(cur, reqSize);
                cur->isFree = false;
                return reinterpret_cast<unsigned char *>(cur) + sizeof(BlockHeader);
            }
            cur = cur->next;
        }

        return nullptr;

    }

    void deallocate(void *ptr) {
        if(!ptr) return;
        unsigned char* ptr_char = reinterpret_cast<unsigned char *>(ptr);
        BlockHeader *block = reinterpret_cast<BlockHeader *>(ptr_char - sizeof(BlockHeader));
        block->isFree = true;
        mergeWithNext(block);
        if(block->prev and block->prev->isFree) mergeWithNext(block->prev); 
    }


};

int main() {
    
    MemoryAllocator allocator(1024);

    // 1. Basic allocation
    void* p1 = allocator.allocate(100);
    void* p2 = allocator.allocate(100);
    void* p3 = allocator.allocate(100);

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(p3 != nullptr);

    assert(p1 != p2);
    assert(p2 != p3);

    // 2. Returned pointers should be max-aligned
    const std::size_t alignment = alignof(std::max_align_t);

    assert(reinterpret_cast<std::uintptr_t>(p1) % alignment == 0);
    assert(reinterpret_cast<std::uintptr_t>(p2) % alignment == 0);
    assert(reinterpret_cast<std::uintptr_t>(p3) % alignment == 0);

    // 3. Memory can be written to
    std::memset(p1, 1, 100);
    std::memset(p2, 2, 100);
    std::memset(p3, 3, 100);

    // 4. Free and reuse the same block
    allocator.deallocate(p2);

    void* p4 = allocator.allocate(100);
    assert(p4 == p2);  // first-fit should reuse p2

    // Restore state for coalescing test
    allocator.deallocate(p4);

    // 5. Coalesce p1 and p2
    allocator.deallocate(p1);

    // Neither original block alone can hold 200 bytes,
    // but the merged block can.
    void* large = allocator.allocate(200);

    assert(large != nullptr);
    assert(large == p1);

    allocator.deallocate(large);
    allocator.deallocate(p3);

    // 6. Out-of-memory case
    void* tooLarge = allocator.allocate(2000);
    assert(tooLarge == nullptr);

    // 7. nullptr deallocation should be harmless,
    // provided deallocate() contains: if (!ptr) return;
    allocator.deallocate(nullptr);

    std::cout << "All allocator tests passed\n";
}
