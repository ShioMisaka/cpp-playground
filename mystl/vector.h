#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <utility>
#include <stdexcept>

namespace mystl
{

template <typename Tp, typename Alloc = std::allocator<Tp>>
class vector
{
public:
    using value_type      = Tp;
    using allocator_type  = Alloc;
    using size_type       = size_t;
    using reference       = value_type &;
    using const_reference = const value_type &;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;

    using iterator        = pointer;
    using const_iterator  = const_pointer;
    
private:
    size_type size_;
    size_type capacity_;
    pointer data_;
    allocator_type allocator_;

public:
    // ========== Constructors / Destructor ==========
    // Default constructor
    vector() noexcept : size_(0), capacity_(0), data_(nullptr) {}

    // Fill constructor (n default-initialized elements)
    explicit vector(size_type n) : size_(n), capacity_(n), data_(nullptr)
    {
        data_ = allocator_.allocate(capacity_);
        for (size_t i = 0; i < size_; ++i)
            std::allocator_traits<allocator_type>::construct(allocator_, data_ + i);
    }

    vector(std::initializer_list<value_type> ilist)
        : size_(ilist.size()), capacity_(ilist.size()), data_(nullptr)
    {
        data_ = allocator_.allocate(capacity_);
        size_type i = 0;
        for (const auto& val : ilist)
            std::allocator_traits<allocator_type>::construct(allocator_, data_ + i++, val);
    }

    // 拷贝构造
    vector(const vector & other)
        : size_(other.size_), capacity_(other.capacity_), data_(nullptr)
    {
        data_ = allocator_.allocate(capacity_);
        for (size_type i = 0; i < size_; ++i)
            std::allocator_traits<allocator_type>::construct(allocator_, data_ + i, std::as_const(other.data_[i]));
    }

    vector(vector && other) noexcept
        : size_(other.size_), capacity_(other.capacity_), data_(other.data_)
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    ~vector()
    {
        clear();
        if(data_) allocator_.deallocate(data_, capacity_);
    }

    // 拷贝赋值
    vector& operator=(vector const& other)
    {
        if (this != &other) {
            clear();
            if (capacity_ < other.capacity_) {
                allocator_.deallocate(data_, capacity_);
                capacity_ = other.capacity_;
                data_ = allocator_.allocate(capacity_);
            }
            size_ = other.size_;
            for (size_type i = 0; i < size_; ++i) {
                std::allocator_traits<allocator_type>::construct(allocator_, data_ + i, std::as_const(other.data_[i]));
            }
        }
        return *this;
    }

    // 移动赋值
    vector& operator=(vector && other) noexcept
    {
        if (this != &other) {
            clear();
            allocator_.deallocate(data_, capacity_);
            capacity_ = other.capacity_;
            size_ = other.size_;
            data_ = other.data_;
            other.data_ = nullptr;
            other.capacity_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    // ========== 元素访问 ==========
    reference at(size_type pos)
    {
        if (pos < size_) return data_[pos];
        else throw std::out_of_range("mystl::vector out of range.");
    }

    const_reference at(size_type pos) const
    {
        if (pos < size_) return data_[pos];
        else throw std::out_of_range("mystl::vector out of range.");
    }

    reference operator[](size_type i) { return data_[i]; }
    const_reference operator[](size_type i) const { return data_[i]; }

    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    iterator data() { return data_; }
    
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }
    const_iterator data() const { return data_; }

    // ========== 容量 ==========
    // 扩容，reserve 只能扩容
    void reserve(size_type n)
    {
        if (n <= capacity_) return;
        pointer new_data = allocator_.allocate(n);
        for (size_type i = 0; i < size_; ++i){
            std::allocator_traits<allocator_type>::construct(allocator_, new_data + i, std::move_if_noexcept(data_[i]));
            std::allocator_traits<allocator_type>::destroy(allocator_, data_ + i);
        }
        if(data_) allocator_.deallocate(data_, capacity_);
        capacity_ = n;
        data_ = new_data;
    }

    void shrink_to_fit()
    {
        if (size_ == capacity_) return;
        pointer new_data = allocator_.allocate(size_);
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<allocator_type>::construct(allocator_, new_data + i, std::move_if_noexcept(data_[i]));
            std::allocator_traits<allocator_type>::destroy(allocator_, data_ + i);
        }
        allocator_.deallocate(data_, capacity_);
        capacity_ = size_;
        data_ = new_data;
    }

    void resize(size_type n) 
    {
        reserve(n);
        for (size_type i = size_; i < n; ++i)
            std::allocator_traits<allocator_type>::construct(allocator_, data_ + i);
        for (size_type i = n; i < size_; ++i)
            std::allocator_traits<allocator_type>::destroy(allocator_, data_ + i);
        size_ = n;
    }

    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    bool empty() const { return size_ == 0 ? true : false; }


    // ========== 修改器 ==========
private:
    template<typename... Args>
    void construct_at_back(Args&&... args)
    {
        if (size_ == capacity_) grow();
        std::allocator_traits<allocator_type>::construct(allocator_, data_ + size_++, std::forward<Args>(args)...);
    }

    void grow() { reserve(capacity_ == 0 ? 1 : capacity_ * 2); }

public:
    // 添加一个数据
    void push_back(const value_type& val) { construct_at_back(val); }

    // 移动添加一个数据
    void push_back(value_type&& val) { construct_at_back(std::move(val)); }

    template<typename... Args>
    void emplace_back(Args&&... args) { construct_at_back(std::forward<Args>(args)...); }

    template<typename... Args>
    void insert(size_type pos, Args&&... args) 
    {
        if (pos > size_) return;
        if (size_ == capacity_) grow();

        for (size_type i = size_; i > pos; --i){
            std::allocator_traits<allocator_type>::construct(allocator_, data_ + i, std::move_if_noexcept(data_[i - 1]));
            std::allocator_traits<allocator_type>::destroy(allocator_, data_ + i - 1);
        }

        std::allocator_traits<allocator_type>::construct(allocator_, data_ + pos, std::forward<Args>(args)...);
        ++size_;
    }

    // 删除一个数据
    void erase(size_type pos)
    {
        if (pos >= size_) return;
        if (pos + 1 < size_) {
            std::move(data_ + pos + 1, data_ + size_, data_ + pos);
        }
        std::allocator_traits<Alloc>::destroy(allocator_, data_ + size_ - 1);
        --size_;
    }

    void pop_back()
    {
        if (size_> 0)
            std::allocator_traits<allocator_type>::destroy(allocator_, data_ + --size_);
    }

    void clear()
    {
        for (size_type i = 0; i < size_; ++i){
            std::allocator_traits<allocator_type>::destroy(allocator_, data_ + i);
        }
       size_ = 0;
    }

};

} // namespace mystl
