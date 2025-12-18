#pragma once

#include "utility.h"

namespace mystl
{

template <typename Tp>
class defult_deleter
{
public:
    void operator()(Tp* p) const {
        delete p;
    }
};

template <typename Tp, typename Deleter = defult_deleter<Tp>>
class unique_ptr
{
public:
    unique_ptr(): data_(nullptr){}
    
    ~unique_ptr() {
        if (data_ != nullptr)
            Deleter{}(data_);
        data_ = nullptr;
    }
    
    explicit unique_ptr(Tp* data) :data_(data){}

    unique_ptr(const Tp &other) = delete;
    unique_ptr &operator=(const Tp &) = delete;

    unique_ptr(unique_ptr<Tp>&& other) noexcept {
        data_ = other.data_;
        other.data_ = nullptr;
    }

    void operator=(unique_ptr<Tp>&& other) noexcept {
        if (this != &other){
            data_ = other.data_;
            other.data_ = nullptr;
        }
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, Tp*>::value>>
    unique_ptr(unique_ptr<U>&& other) noexcept : data_(other.release()) {}

    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, Tp*>::value>>
    unique_ptr& operator=(unique_ptr<U>&& other) noexcept {
        reset(other.release());
        return *this;
    }

    Tp* operator->() const {
        return data_;
    }

    Tp& operator*() const {
        return *data_;
    }

    Tp* get() const {
        return data_;
    }

    Tp* release() {
        Tp *temp = data_;
        data_ = nullptr;
        return temp;
    }

    void reset(Tp* ptr = nullptr) {
        if (data_ != nullptr) {
            Deleter{}(data_);
        }
        data_ = ptr;
    }

private:
    Tp *data_;
};

template <typename Tp, typename... Arg>
unique_ptr<Tp> make_unique(Arg&&... arg) {
    return unique_ptr<Tp>(new Tp(forward<Arg>(arg)...));
}

} // namespace 

