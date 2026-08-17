#include <cstddef>
#include <new>

template <
    std::size_t BlockSize,
    std::size_t BlockCount,
    std::size_t Alignment = alignof(std::max_align_t)
>
class FixedMemoryPool {
private:
    struct FreeNode {
        FreeNode* next;
    };

    static constexpr std::size_t RequiredSize =
        BlockSize > sizeof(FreeNode) ? BlockSize : sizeof(FreeNode);

    static constexpr std::size_t RequiredAlignment =
        Alignment > alignof(FreeNode) ? Alignment : alignof(FreeNode);

    // Each Slot represents one allocatable block.
    struct alignas(RequiredAlignment) Slot {
        std::byte memory[RequiredSize];
    };

    Slot storage_[BlockCount];
    FreeNode* freeHead_ = nullptr;

public:
    FixedMemoryPool() noexcept {
        // Initially, every block is free.
        for (std::size_t i = 0; i < BlockCount; ++i) {
            void* block = storage_[i].memory;

            auto* node = new (block) FreeNode{freeHead_};
            freeHead_ = node;
        }
    }

    FixedMemoryPool(const FixedMemoryPool&) = delete;
    FixedMemoryPool& operator=(const FixedMemoryPool&) = delete;

    void* allocate() noexcept {
        if (!freeHead_) {
            return nullptr;
        }

        FreeNode* block = freeHead_;
        freeHead_ = freeHead_->next;

        return block;
    }

    void deallocate(void* block) noexcept {
        if (!block) {
            return;
        }

        // The caller must pass a block previously returned by this pool.
        auto* node = new (block) FreeNode{freeHead_};
        freeHead_ = node;
    }
};

