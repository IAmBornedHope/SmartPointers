#pragma once

#include <cstddef>
#include <utility>
#include "ControlBlock.hpp"

template<typename T>
class SharedPtr {
private:
    using Type = std::remove_extent_t<T>;
    ControlBlockBase* block_ = nullptr;
    Type* pointer_ = nullptr;

    void release_block() {
        if (block_) {
            block_->ref_counter--;
            if (block_->ref_counter == 0) {
                delete block_;
            }
        }
        block_ = nullptr;
        pointer_ = nullptr;
    }
public:
    SharedPtr() = default;

    SharedPtr(std::nullptr_t) noexcept : SharedPtr() {}

    template<typename U>
        requires(!std::is_array_v<U> && std::is_convertible_v<U*, T*>)
    SharedPtr(U* ptr) {
        if (ptr) {
            try {
                block_ = new ControlBlock<U>(ptr);
                pointer_ = ptr;
            } catch (...) {
                delete ptr;
                throw;
            }
   
        }
    }

    SharedPtr(Type* ptr) requires(std::is_array_v<T>) {
        if (ptr) {
            block_ = new ControlBlock<T>(ptr);
            pointer_ = ptr;
        }
    }

    template<typename U>
        requires(std::is_convertible_v<U*, T*>)
    SharedPtr(const SharedPtr<U>& other) : block_(other.block_), pointer_(other.get()) 
    {
        if (block_) {
            block_->ref_counter++;
        }
    }

    template<typename U>
        requires(std::is_convertible_v<U*, T*>)
    SharedPtr(SharedPtr<U>&& other) : block_(other.block_), pointer_(other.pointer_) 
    {
        other.block_ = nullptr;
        other.pointer_ = nullptr;
    }

    SharedPtr(const SharedPtr& other) : block_(other.block_), pointer_(other.pointer_) 
    {
        if (block_) {
            block_->ref_counter++;
        }
    }

    SharedPtr(SharedPtr&& other) : block_(other.block_), pointer_(other.pointer_) 
    {
        other.block_ = nullptr;
        other.pointer_ = nullptr;
    }

   SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release_block();
            block_ = other.block_;
            pointer_ = other.pointer_;
            other.block_ = nullptr;
            other.pointer_ = nullptr;
        }
        return *this;
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (block_ != other.block_) {
            if (other.block_) {
                other.block_->ref_counter++;
            }
            release_block();
            block_ = other.block_;
            pointer_ = other.pointer_;
        }
        return *this;
    }

    ~SharedPtr() {
        release_block();
    }

    Type* get() const noexcept {
        return pointer_;
    }

    Type& operator*() const noexcept requires (!std::is_array_v<T>) {
        return *pointer_;
    }

    Type* operator->() const noexcept requires (!std::is_array_v<T>) {
        return pointer_;
    }

    Type& operator[](std::size_t index) const requires (std::is_array_v<T>){
        return pointer_[index];
    }

    std::size_t get_counter() const noexcept {
        if (block_) {
            return block_->ref_counter;
        }
        return 0;
    }

    explicit operator bool() const noexcept {
        return pointer_ != nullptr;
    }


    void reset() noexcept {
        release_block();
    }
    
    template<typename U>
        requires (!std::is_array_v<U>)
    void reset(U* ptr) {
        if (ptr != pointer_) {
            release_block();
            if (ptr) {
                try {
                    block_ = new ControlBlock<U>(ptr);
                    pointer_ = ptr;
                } catch (...) {
                    delete ptr;
                    throw;
                }

            }
        }
    }

    void reset(Type* ptr) requires (std::is_array_v<T>) {
        if (ptr != pointer_) {
            release_block();
            if (ptr) {
                try {
                    block_ = new ControlBlock<T>(ptr);
                    pointer_ = ptr;
                } catch (...) {
                    delete[] ptr;
                    throw;
                }

            }
        }
    }

    void reset(std::nullptr_t) noexcept {
        release_block();
    }

    template<typename U>
    friend class SharedPtr;

};

template<typename T, typename... Args>
    requires(!std::is_array_v<T>)
SharedPtr<T> make_shared(Args&&... args) {
    T* pointer = new T(std::forward<Args>(args)...);
    try {
        return SharedPtr<T>(pointer);
    } catch (...) {
        delete pointer;
        throw;
    }
}

template<typename T>
    requires(std::is_array_v<T>)
SharedPtr<T> make_shared(std::size_t size) {
    using Type = std::remove_extent_t<T>;
    Type* pointer = new Type[size]();
    try {
        return SharedPtr<T>(pointer);
    } catch (...) {
        delete[] pointer;
        throw;
    }
}
