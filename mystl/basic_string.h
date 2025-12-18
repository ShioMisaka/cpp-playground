#pragma once

#include <iostream>
#include <cstring>     // For std::strlen, std::memcpy, etc.
#include <memory>
#include "vector.h"

namespace mystl
{
template<typename CharT, typename Traits = std::char_traits<CharT>, typename Alloc = std::allocator<CharT>>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template<typename CharT, typename Traits, typename Alloc>
class basic_string
{
public:
    using value_type      = CharT;
    using allocator_type  = Alloc;
    using traits_type     = Traits;
    using size_type       = size_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = pointer;
    using const_iterator  = const_pointer;
      
    static const size_type npos = static_cast<size_type>(-1);

private:
    static const size_type SSO_CAPACITY = 15;

    union 
    {
        pointer data_;
        value_type sso_buf_[SSO_CAPACITY + 1];
    };

    size_type size_;
    size_type capacity_;
    allocator_type allocator_;

private:
    // ========== Private Helpers ==========

    // Checks if the string is in SSO mode.
    bool is_short() const noexcept { return capacity_ == SSO_CAPACITY; }

    void set_short_size(size_type n)
    {
        size_ = n;
        capacity_ = SSO_CAPACITY;
        sso_buf_[size_] = '\0';
    }

    void set_long_size(size_type n)
    {
        size_ = n;
        capacity_ = n;
        data_ = allocator_.allocate(capacity_ + 1);
        data_[size_] = '\0';
    }

    // A single helper to construct the string from a C-style string.
    void construct_from_char_ptr(const char* str, size_type len)
    {
        if (len <= SSO_CAPACITY) {
            set_short_size(len);
            traits_type::copy(sso_buf_, str, len + 1);
        } else {
            set_long_size(len);
            traits_type::copy(data_, str, len + 1);
        }
    }

    // Grows the buffer if needed to accommodate n additional characters.
    void smart_grow(size_type n = 1)
    {
        if (size_ + n > capacity_) {
            size_type new_cap = std::max(capacity_ * 2, size_ + n);
            reserve(new_cap);
        }
    }

    // Gets a pointer to the beginning of the character sequence.
    pointer get_current_data() { return is_short() ? sso_buf_ : data_; }
    const_pointer get_current_data() const { return is_short() ? sso_buf_ : data_; }

    // Deallocates the long string buffer if it exists.
    void deallocate_long() noexcept
    {
        if (!is_short()) {
            allocator_.deallocate(data_, capacity_ + 1);
        }
    }

    size_type kmp_search(const basic_string& patt, size_type pos)
    {
        std::vector<int> lps(patt.size(), 0);
        for (size_type i = 1, len = 0; i < patt.size();) {
            if (patt[i] == patt[len]) {
                lps[i++] = ++len;
            } else if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i++] = 0;
            }
        }
        size_type i = pos;
        size_type j = 0;
        while (i < size_) {
            if (data_[i] == patt[j]) { 
                ++i; ++j;
                if (j == patt.size()) return i - j;
            }
            else if (j > 0) { j = lps[j - 1]; }
            else { ++i; }
        }
        return npos;
    }

    

public:
    // ========== Constructors, Destructor, Assignment ==========
    basic_string(): size_(0), capacity_(0), data_(nullptr) 
    {
        set_short_size(0);
    }

    basic_string(const_pointer str, size_type len)
    {
        construct_from_char_ptr(str, len);
    }

    basic_string(const_pointer str)
    {
        size_type len = traits_type::length(str);
        construct_from_char_ptr(str, len);
    }

    basic_string(const basic_string& other)
    {
        construct_from_char_ptr(other.get_current_data(), other.size_);
    }

    basic_string(basic_string&& other) noexcept
        : size_(other.size_), capacity_(other.capacity_), allocator_(std::move(other.allocator_))
    {
       if (other.is_short()) {
            traits_type::copy(sso_buf_, other.sso_buf_, other.size_ + 1);
        } else {
            data_ = other.data_;
            other.data_ = nullptr;
        }
        other.set_short_size(0);
    }

    ~basic_string()
    {
        deallocate_long();
    }

    void swap(basic_string& other) noexcept
    {
        basic_string temp = std::move(*this);
        *this = std::move(other);
        other = std::move(temp);
    }

    basic_string& operator=(const basic_string& other)
    {
        if (this == &other) return *this;
        basic_string temp(other);
        swap(temp);
        return *this;
    }

    basic_string& operator=(basic_string&& other) noexcept
    {
        if (this != &other)
        {
            deallocate_long();

            size_ = other.size_;
            capacity_ = other.capacity_;
            allocator_ = std::move(other.allocator_);

            if (other.is_short()) {
                traits_type::copy(sso_buf_, other.sso_buf_, other.size_ + 1);
            } else {
                data_ = other.data_;
                other.data_ = nullptr;
            }

            other.set_short_size(0);
        }
        return *this;
    }

    // ========== Iterators ==========
    iterator begin() noexcept { return get_current_data(); }
    const_iterator begin() const noexcept { return get_current_data(); }

    iterator end() noexcept { return get_current_data() + size_; }
    const_iterator end() const noexcept { return get_current_data() + size_; }

    // ========== Element access ==========
    reference operator[](size_type pos) { return get_current_data()[pos]; }
    const_reference operator[](size_type pos) const { return get_current_data()[pos]; }

    reference at(size_type pos)
    {
        if (pos < size_)
            return get_current_data()[pos];
        else
            std::out_of_range("mystl::string out of range.");
    }

    const_reference at(size_t pos) const
    {
        if (pos < size_)
            return get_current_data()[pos];
        else
            std::out_of_range("mystl::string out of range.");
    }
    
    const_pointer c_str() const noexcept { return get_current_data(); }

    pointer data() { return get_current_data(); }
    const_pointer data() const { return get_current_data(); }
    
    // ========== Capacity ==========
    size_type size() const noexcept { return size_; }
      size_type length() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    void reserve(size_type new_cap)
    {
        if (new_cap < capacity_) return;

        pointer new_data = allocator_.allocate(new_cap + 1);
        pointer old_data = get_current_data();

        traits_type::copy(new_data, old_data, size_ + 1);

        deallocate_long();
        capacity_ = new_cap;
        data_ = new_data;
    }

    void resize(size_type new_size, value_type c = value_type())
    {
        if (new_size > size_) {
            reserve(new_size);
            pointer p = get_current_data();
            for (size_t i = size_; i < new_size; ++i) {
                p[i] = c;
            }
            size_ = new_size;
            p[size_] = value_type();
        }
        size_ = new_size;
        get_current_data()[size_] = value_type();
    }

    basic_string substr(size_type begin = 0, size_type len = -1)
    {
        const_pointer pBegin;
        if (is_short())
            pBegin = sso_buf_ + begin;
        else
            pBegin = data_ + begin;
        if (len == -1)
            len = size_ - begin;
        return basic_string(pBegin, len);
    }

    size_type find(const basic_string& patt, size_type pos = 0)
    {
        return kmp_search(patt, pos);
    }

    // ========== Modifiers ==========
    void clear() noexcept
    {
        size_ = 0;
        get_current_data()[size_] = value_type();
    }

    void push_back(value_type ch)
    {
        smart_grow(1);
        pointer p = get_current_data();
        p[size_++] = ch;
        p[size_] = value_type();
    }

    basic_string& append(const_pointer str, size_type n)
    {
        smart_grow(n);
        pointer pCur = get_current_data();
        traits_type::copy(pCur + size_, str, n);
        size_ += n;
        pCur[size_] = value_type();
        return *this;
    }
    basic_string& append(const_pointer str) { return append(str, traits_type::length(str)); }
    basic_string& append(const basic_string& str) { return append(str.get_current_data(), str.size_); }

    basic_string& operator+=(const value_type c) { return push_back(c); }
    basic_string& operator+=(const_pointer str) { return append(str); }
    basic_string& operator+=(const basic_string &str) { return append(str); }


    friend std::ostream& operator<<(std::ostream& os, const basic_string& str) {
        return os.write(str.get_current_data(), str.size());
    }

    friend basic_string operator+(const basic_string& lhs, const_pointer rhs)
    {
        basic_string result(lhs);
        result.append(rhs);
        return result;
    }

    friend basic_string operator+(const basic_string& lhs, const basic_string& rhs)
    {
        basic_string result(lhs);
        result.append(rhs);
        return result;
    }
};

// ========== Non-member functions ==========
template<typename CharT, typename Traits, typename Alloc>
basic_string<CharT, Traits, Alloc> operator<<(std::ostream& os, const basic_string<CharT, Traits, Alloc>& str)
{
    return os.write(str.data(), str.size());
}


template<typename CharT, typename Traits, typename Alloc>
basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    basic_string<CharT, Traits, Alloc> result;
    result.reserve(lhs.size() + rhs.size());
    result.append(lhs);
    result.append(rhs);
    return result;
}

template<typename CharT, typename Traits, typename Alloc>
basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
{
    basic_string<CharT, Traits, Alloc> result(lhs);
    result.append(rhs);
    return result;
}

template<typename CharT, typename Traits, typename Alloc>
bool operator==(const basic_string<CharT, Traits, Alloc>& lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return (lhs.size() == rhs.size()) && (Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0);
}

template<typename CharT, typename Traits, typename Alloc>
bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs, const basic_string<CharT, Traits, Alloc>& rhs) noexcept
{
    return !(lhs == rhs);
}

namespace literals{
inline string operator""_s(const char* str, size_t len)
{
    return string(str, len);
}
}// namespace literals

}// namespace mystl