#pragma once

#include <type_traits>

template<typename T>
class UniquePtr {
    using Type = std::remove_extent_t<T>;

private:
    Type* pointer_ = nullptr;

    void destroy(Type* pointer) {
        if constexpr(std::is_array_v<T>) {
            delete[] pointer;
        } else {
            delete pointer;
        }
    }

public:
    UniquePtr() = default;
    UniquePtr(Type* pointer) : pointer_(pointer) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : pointer_(other.pointer_) {
        other.pointer_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        reset(other.release());
        return *this;
        
    }

    ~UniquePtr() {
        destroy(pointer_);
    }

    Type* release() {
        Type* temp = pointer_;
        pointer_ = nullptr;
        return temp;
    }

    void reset (Type* p = nullptr) {
        if (pointer_ != p) {
            destroy(pointer_);
            pointer_ = p;
        }
    }

};