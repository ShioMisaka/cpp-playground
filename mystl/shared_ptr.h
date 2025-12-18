#pragma once

#include <atomic>
#include <utility>

#include "utility.h"

namespace mystl
{

namespace detail
{
class ctrl_block_base 
{
public:
    std::atomic<long> shared_count_{1};
    std::atomic<long> weak_count_{1};

    virtual void destroy_object() = 0;
    virtual void destroy_ctrl_block() = 0;
    virtual ~ctrl_block_base() = default;
};

template<typename T>
class ctrl_block_ptr : public ctrl_block_base
{
public:
    T *data_;

    explicit ctrl_block_ptr(T* data) : data_(data) {}

    virtual void destroy_object() override {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }
    }

    virtual void destroy_ctrl_block() override {
        delete this;
    }
};

template<typename T>
class ctrl_block_inplace : public ctrl_block_base
{
private:
    alignas(T) unsigned char storage_[sizeof(T)];
    bool constructed_{false};

public:
    template<typename... Args>
    explicit ctrl_block_inplace(Args&&... args) {
        new (storage_) T(forward<Args>(args)...);
        constructed_ = true;
    }

    T *get_ptr() { return reinterpret_cast<T *>(storage_); }

    void destroy_object() override {
        if (constructed_) {
            get_ptr()->~T();
            constructed_ = false;
        }
    }

    void destroy_ctrl_block() override {
        delete this;
    }
};

inline void incref_shared(ctrl_block_base* cb) {
    if (cb) cb->shared_count_.fetch_add(1, std::memory_order_relaxed);
}

inline void decref_shared(ctrl_block_base* cb) {
    if (!cb) return;

    if (cb->shared_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        cb->destroy_object();

        if (cb->weak_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            cb->destroy_ctrl_block();
        }
    }
}

inline void incref_weak(ctrl_block_base* cb) {
    if (cb) cb->weak_count_.fetch_add(1, std::memory_order_relaxed);
}

inline void decref_weak(ctrl_block_base* cb) {
    if (!cb) return;

    if (cb->weak_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        cb->destroy_ctrl_block();
    }
}

} // detail

template <typename Tp> class weak_ptr;

template <typename Tp>
class shared_ptr
{
    template <typename U> friend class shared_ptr;
    template <typename U> friend class weak_ptr;

private:
    Tp *ptr_;
    detail::ctrl_block_base *ctrl_block_;

public:
    shared_ptr() noexcept : ptr_(nullptr), ctrl_block_(nullptr) {};

    explicit shared_ptr(Tp *data) 
        : ptr_(data), ctrl_block_(data ? new detail::ctrl_block_ptr<Tp>(data) : nullptr) {};

    shared_ptr(const shared_ptr& that) noexcept
        : ptr_(that.ptr_), ctrl_block_(that.ctrl_block_) {
        detail::incref_shared(ctrl_block_);
    }

    template<typename U>
    shared_ptr(const shared_ptr<U>& that) noexcept 
        : ptr_(that.ptr_), ctrl_block_(that.ctrl_block_) {
        detail::incref_shared(ctrl_block_);
    }

    shared_ptr(shared_ptr&& that) noexcept  
        : ptr_(that.ptr_), ctrl_block_(that.ctrl_block_) {
        that.ptr_ = nullptr;
        that.ctrl_block_ = nullptr;
    }

    template<typename U>
    shared_ptr(shared_ptr<U>&& that) noexcept
        : ptr_(that.ptr_), ctrl_block_(that.ctrl_block_) {
        that.ptr_ = nullptr;
        that.ctrl_block_ = nullptr;
    }

    explicit shared_ptr(const weak_ptr<Tp>& wp);

    shared_ptr& operator=(const shared_ptr& that) noexcept {
        if (this != &that) {
            detail::decref_shared(ctrl_block_);
            ptr_ = that.ptr_;
            ctrl_block_ = that.ctrl_block_;
            detail::incref_shared(ctrl_block_);
        }
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& that) noexcept {
        if (this != &that) {
            detail::decref_shared(ctrl_block_);
            ptr_ = that.ptr_;
            ctrl_block_ = that.ctrl_block_;
            that.ptr_ = nullptr;
            that.ctrl_block_ = nullptr;
        }
        return *this;
    }

    ~shared_ptr() {
        detail::decref_shared(ctrl_block_);
    }

    Tp* get() const { return ptr_; }
    Tp* operator->() const { return ptr_; }
    Tp& operator*() const { return *ptr_; }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    long use_count() const noexcept {
        return ctrl_block_ ? ctrl_block_->shared_count_.load(std::memory_order_relaxed) : 0;
    }

    bool unique() const noexcept {
        return this->use_count() == 1;
    }

    void reset() {
        shared_ptr().swap(*this);
    }

    void reset(Tp* ptr) {
        shared_ptr(ptr).swap(*this);
    }

    void swap(shared_ptr& that) {
        std::swap(ptr_, that.ptr_);
        std::swap(ctrl_block_, that.ctrl_block_);
    }

private:
    template<typename U>
    shared_ptr(U* ptr, detail::ctrl_block_base* cb) noexcept
        : ptr_(ptr), ctrl_block_(cb) {}

    template <typename U, typename... Args>
    friend shared_ptr<U> make_shared(Args&&...args);
};

template<typename Tp>
class weak_ptr
{
    template<typename U> friend class weak_ptr;
    template<typename U> friend class shared_ptr;

private:
    Tp *ptr_;
    detail::ctrl_block_base *ctrl_block_;

public:
    weak_ptr() noexcept : ptr_(nullptr), ctrl_block_(nullptr) {}

    weak_ptr(const weak_ptr& that) noexcept
        : ptr_(that.ptr_), ctrl_block_(that.ctrl_block_) {
        detail::incref_weak(ctrl_block_);
    }

    weak_ptr(weak_ptr&& that) noexcept
        : ptr_(that.ptr_), ctrl_block_(that.ctrl_block_) {
        detail::incref_weak(ctrl_block_);
    }

    weak_ptr& operator=(const weak_ptr& that) {
        if (this != &that) {
            detail::decref_weak(ctrl_block_);
            ptr_ = that.ptr_;
            ctrl_block_ = that.ctrl_block_;
            detail::incref_weak(ctrl_block_);
        }
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& that) {
        if (this != &that) {
            detail::decref_weak(that);
            ptr_ = that.ptr_;
            ctrl_block_ = that.ctrl_block_;
            that.ptr_ = nullptr;
            that.ctrl_block_ = nullptr;
        }
        return *this;
    }

    weak_ptr(const shared_ptr<Tp>& sp) noexcept
        : ptr_(sp.ptr_), ctrl_block_(sp.ctrl_block_) {
        detail::incref_weak(ctrl_block_);
    }

    weak_ptr& operator=(const shared_ptr<Tp>& sp) noexcept {
        detail::decref_shared(ctrl_block_);
        ptr_ = sp.ptr_;
        ctrl_block_ = sp.ctrl_block_;
        detail::incref_weak(ctrl_block_);
        return *this;
    }

    ~weak_ptr() {
        detail::decref_weak(ctrl_block_);
    }

    bool expired() const noexcept {
        return !ctrl_block_ || ctrl_block_->shared_count_.load(std::memory_order_relaxed) == 0;
    }

    long use_count() const noexcept {
        return ctrl_block_ ? ctrl_block_->shared_count_.load(std::memory_order_relaxed) : 0;
    }

    void reset() {
        weak_ptr().swap(*this);
    }

    shared_ptr<Tp> lock() const noexcept {
        if (!ctrl_block_) return shared_ptr<Tp>();

        long old_count = ctrl_block_->shared_count_.load(std::memory_order_relaxed);
        do { 
            if (old_count == 0) return shared_ptr<Tp>();
        } while (ctrl_block_->shared_count_.compare_exchange_weak(old_count, old_count + 1, 
            std::memory_order_acquire, std::memory_order_relaxed));

        return shared_ptr<Tp>(ptr_, ctrl_block_);
    }

    void swap(weak_ptr& that) {
        std::swap(ptr_ ,that.ptr_);
        std::swap(ctrl_block_, that.ctrl_block_);
    }
};

template<typename Tp>
shared_ptr<Tp>::shared_ptr(const weak_ptr<Tp>& wp)
    : ptr_(nullptr), ctrl_block_(nullptr) {
    auto sp = wp.lock();
    if (sp) {
        ptr_ = sp.ptr_;
        ctrl_block_ = sp.ctrl_block_;
        sp.ptr_ = nullptr;
        sp.ctrl_block_ = nullptr;
    }
}

template <typename Tp, typename... Args>
shared_ptr<Tp> make_shared(Args&&... args) {
    auto *cb = new detail::ctrl_block_inplace<Tp>(std::forward<Args>(args)...);
    return shared_ptr<Tp>(cb->get_ptr(), cb);
}

} // mystl