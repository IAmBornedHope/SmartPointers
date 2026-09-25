#pragma once

#include <cstddef>
#include <type_traits>

class ControlBlockBase {
public:
    std::size_t ref_counter = 1;
    virtual ~ControlBlockBase() = default;
};

template<typename T>
class ControlBlock : public ControlBlockBase {
    using Type = std::remove_extent_t<T>;
    Type* pointer_ = nullptr;

    void destroy(Type* pointer) {
        if constexpr(std::is_array_v<T>) {
            delete[] pointer;
        } else {
            delete pointer;
        }
    }

public:
    ControlBlock(Type* pointer): pointer_(pointer) {}
    ~ControlBlock() override {
        destroy(pointer_);
    }

    ControlBlock(const ControlBlock&) = delete;
    ControlBlock& operator=(const ControlBlock&) = delete;

};