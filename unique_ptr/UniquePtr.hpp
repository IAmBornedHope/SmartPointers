#pragma once

#include <cstddef>
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

    Type* release() noexcept {
        Type* temp = pointer_;
        pointer_ = nullptr;
        return temp;
    }

    void reset (Type* ptr = nullptr) {
        if (pointer_ != ptr) {
            destroy(pointer_);
            pointer_ = ptr;
        }
    }

    Type* get() const {
        return pointer_;
    }

    Type& operator*() const requires(!std::is_array_v<T>) {
        return *pointer_;
    }
    
    Type& operator[](std::size_t index) const requires(std::is_array_v<T>) {
        return pointer_[index];
    }

    Type* operator->() const requires(!std::is_array_v<T>) {
        return pointer_;
    }

    explicit operator bool() const noexcept {
        return pointer_ != nullptr;
    }
    ~UniquePtr() {
        destroy(pointer_);
    }
};


template<typename T, typename... Args>
        requires(!std::is_array_v<T>)
UniquePtr<T> make_unique(Args&&... args) {
    using Type = std::remove_extent_t<T>;
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
        requires(std::is_array_v<T>)
UniquePtr<T> make_unique(std::size_t size) {
    using Type = std::remove_extent_t<T>;
    return UniquePtr<T>(new Type[size]{});
}
