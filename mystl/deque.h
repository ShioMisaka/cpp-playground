#pragma once

#include <memory>
#include <utility>

#include <cassert>

namespace mystl
{

static constexpr size_t MYSTL_DEQUE_BUFFER_SIZE = 4;
static constexpr size_t MYSTL_DEQUE_BUFFER_NUM = 4;

template <typename Tp>
struct deque_iterator
{
    using self            = deque_iterator;
    using value_type      = Tp;
    using size_type       = size_t;
    using pointer         = Tp *;
    using reference       = Tp &;
    using difference_type = std::ptrdiff_t;

    pointer cur_;     // 当前元素
    pointer first_;   // 当前 buffer 的起始
    pointer last_;    // 当前 buffer 的末尾
    pointer *node_;   // 指向 map 中的 buffer 指针

    deque_iterator(): cur_(nullptr), first_(nullptr), last_(nullptr), node_(nullptr) {}
    deque_iterator(pointer cur, pointer first, pointer last, pointer* node)
        :cur_(cur), first_(first), last_(last), node_(node) {}

    reference operator*() { return *cur_; }
    pointer operator->() { return cur_; }

    self& operator++()
    {
        ++cur_;
        if (cur_ == last_) {
            set_node(node_ + 1);
            cur_ = first_;
        }
        return *this;
    }

    self operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    self& operator--()
    {
        if (cur_ == first_) {
            set_node(node_ - 1);
            cur_ = last_;
        }
        --cur_;
        return *this;
    }

    self operator--(int)
    {
        auto temp = *this;
        --*this;
        return temp;
    }

    self& operator+=(size_type i)
    {
        size_type offset = i > 0 ? i + (cur_ - first_) : i - (last_ - cur_);

        if (offset > -MYSTL_DEQUE_BUFFER_SIZE && offset < MYSTL_DEQUE_BUFFER_SIZE) {
            cur_ += i;
        } else {
            size_type node_offset = offset / MYSTL_DEQUE_BUFFER_SIZE;
            size_type buf_offset = offset % MYSTL_DEQUE_BUFFER_SIZE;
            set_node(node_ + node_offset);
            cur_ = i > 0 ? first_ + buf_offset : last_ - buf_offset - 1;
        }
        return *this;
    }

    self& operator-=(size_type i) { return *this += -i; }

    self operator+(size_type i) { self temp = *this; return temp += i; }
    self operator-(size_type i) const  { self temp = *this; return temp -= i; }

    friend difference_type
    operator-(const self& x, const self& y) noexcept
    {
        return static_cast<difference_type>(x.node_ - y.node_) * MYSTL_DEQUE_BUFFER_SIZE
            + (x.cur_ - x.first_) - (y.cur_ - y.first_); 
    }

    friend bool operator==(const self& x, const self& y) { return x.cur_ == y.cur_; }
    friend bool operator!=(const self& x, const self& y) { return x.cur_ != y.cur_; }

    void set_node(pointer* new_node)
    {
        node_ = new_node;
        if (cur_ == first_) cur_ = *new_node;
        first_ = *new_node;
        last_ = first_ + MYSTL_DEQUE_BUFFER_SIZE;
    }
};


template <typename Tp, typename Alloc = std::allocator<Tp>>
class deque
{
public:
    using value_type      = Tp;
    using allocator_type  = Alloc;
    using size_type       = size_t;
    using reference       = value_type &;
    using const_reference = const value_type &;
    using pointer         = value_type *;
    using const_pointer   = const value_type *;
    using difference_type = std::ptrdiff_t;
    using iterator        = deque_iterator<value_type>;
    using const_iterator  = const deque_iterator<value_type>;

public: // private
    pointer*  map_;       // buffer 数组的指针
    size_type map_size_;  // map 的容量
    iterator  start_;     // 起始迭代器
    iterator  finish_;    // 结束迭代器
    size_type sz_;        // 元素个数

    allocator_type allocator_;

public:
    // ========== Constructors / Destructor ==========
    // Default constructor
    deque() : map_(nullptr), map_size_(0), sz_(0)
    {
        allocate_map_and_nodes(0);
    }

    deque(size_type n, const_reference elem) : map_(nullptr), map_size_(0), sz_(n)
    {
        allocate_map_and_nodes(n);

        for (iterator it = start_; it != finish_; ++it) {
            std::allocator_traits<allocator_type>::construct(allocator_, it.cur_, elem);
        }
    }

    deque(const deque& other)
    {
        create_map_and_node(other.size());
        std::uninitialized_copy(other.begin(), other.end(), start_);
    }

    deque(deque&& other) noexcept
        : map_(other.map_), map_size_(other.map_size_),
        start_(other.start_), finish_(other.finish_),
        allocator_(std::move(other.allocator_))
    {
        other.map_ = nullptr;
        other.map_size_ = 0;
    }

    ~deque()
    {
        if (map_) {
            for (size_type i = 0; i < map_size_; ++i) {
                deallocate_buffer(map_[i]);
            }
            delete[] map_;
        }
    }

    deque& operator=(deque& other) noexcept
    {
        
    }

public: // private
    pointer allocate_buffer() { return allocator_.allocate(MYSTL_DEQUE_BUFFER_SIZE); }

    void deallocate_buffer(pointer buf) { if (buf) allocator_.deallocate(buf, MYSTL_DEQUE_BUFFER_SIZE); }

    pointer* allocate_map(size_type map_size) { return new pointer[map_size]; }

    void allocate_map_and_nodes(size_type n);

    // 获取目前使用的节点数量
    size_type node_num() { return finish_.node_ - start_.node_; }

    void reallocate_map(size_type add_num, bool add_at_front);

    void reallocate_map_back(size_type add_num);

    void reallocate_map_front(size_type add_num);

public:
    // ========== 迭代 ==========
    iterator begin() { return start_; }
    const_iterator begin() const { return start_; }
    const_iterator cbegin() const { return start_; }

    iterator end() { return finish_; }
    const_iterator end() const { return finish_; }
    const_iterator cend() const { return finish_; }

    // ========== 容量 ==========
    size_type size() const { return sz_; }

    bool empty() const { return sz_ == 0; }

    size_type capacity() const { return map_size_ * MYSTL_DEQUE_BUFFER_SIZE; }

    // ========== 元素访问 ==========
    reference operator[](size_type i) { return *(start_ + i); }
    const_reference operator[](size_type i) const { return *(start_ + i); }

    reference at(size_type i) 
    {
        if (i < size()) return (*this)[i];
        else throw std::out_of_range("mystl::deque out of range.");
    }

    const_reference at(size_type i) const
    {
        if (i < size()) return (*this)[i];
        else throw std::out_of_range("mystl::deque out of range.");
    }

    reference front() { return *start_; }
    const_reference front() const { return *start_; }

    reference back() { iterator temp = finish_; return *(--temp);}
    const_reference back() const { iterator temp = finish_; return *(--temp);}

    // ========== 容器操作 ==========
private:
    template <typename... Args>
    void construct_back(Args &&...args);

    template <typename... Args>
    void construct_front(Args &&...args);

public:
    void push_back(const value_type &val);
    void push_back(value_type &&val);

    void push_front(const value_type &val);
    void push_front(value_type &&val);

    void pop_back()
    {
        if (sz_ > 0){
            std::allocator_traits<allocator_type>::destroy(allocator_, &(*(--finish_)));
            --sz_;
        }
    }
    void pop_front()
    {
        if (sz_ > 0) {
            std::allocator_traits<allocator_type>::destroy(allocator_, &(*(start_++)));
            --sz_;
        }
    }

    void clear()
    {
        for (iterator it = start_; it != finish_; ++it) {
            std::allocator_traits<allocator_type>::destroy(allocator_, &(*(it)));
        }

        start_.set_node(map_ + map_size_ / 2);
        finish_ = start_;
        sz_ = 0;
    }

    void printf_struct() const;
};

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::allocate_map_and_nodes(size_type n)
{
    // 设置需要的buffer数量
    size_type num_nodes = n / MYSTL_DEQUE_BUFFER_SIZE;
    num_nodes += (n % MYSTL_DEQUE_BUFFER_SIZE) == 0 ? 0 : 1; // buffer不能少于一个
    map_size_ = std::max<size_type>(MYSTL_DEQUE_BUFFER_NUM, num_nodes + 2);

    map_ = allocate_map(map_size_); // 创建新的 map 空间

    for (size_type i = 0; i < map_size_; ++i) {
        map_[i] = allocate_buffer();
    }

    pointer *start_node = map_ + (map_size_ - num_nodes) / 2;

    start_.set_node(start_node);
    start_.cur_ = start_.first_;
    finish_ = start_ + n;
}

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::reallocate_map(size_type add_num, bool add_at_front)
{
    size_type new_map_size =  map_size_ + add_num;
    pointer *new_map = allocate_map(new_map_size); // 扩充map

    size_type add_offset = add_at_front ? add_num : 0; // 判断留位置留在头还是尾

    std::copy(map_, map_ + map_size_, new_map + add_offset); // 复制全部 buffer 指针
    
    // 计算新的 start 位置
    const size_type start_offset = start_.node_ - map_;
    const size_type node_n = node_num();

    delete[] map_;

    map_size_ = new_map_size;
    map_ = new_map;
    start_.set_node(map_ + start_offset + add_offset);
    finish_.set_node(start_.node_ + node_n);

    // 最后添加分配好的 buffer
    size_type add_pos = add_at_front ? 0 : map_size_ - 1; // 加在后面或者前面，前面已经预留好了空间
    for (size_type i = 0; i < add_num; ++i) {
        pointer temp = allocate_buffer();
        map_[add_pos + i] = temp; // 加一个新的 buffer
    }
}

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::reallocate_map_back(size_type add_num)
{
    size_type new_map_size = map_size_ + add_num;
    pointer *new_map = allocate_map(new_map_size);

    std::copy(map_, map_ + map_size_, new_map);

    const size_type start_offset = start_.node_ - map_;
    const size_type node_n = finish_.node_ - start_.node_;

    delete[] map_;

    map_size_ = new_map_size;
    map_ = new_map;
    
    // 最后添加分配好的 buffer
    size_type add_pos = map_size_ - add_num;
    for (size_type i = 0; i < add_num; ++i) {
        pointer temp = allocate_buffer();
        map_[add_pos + i] = temp; // 加一个新的 buffer
    }
    
    // 设置新的控制中心
    start_.set_node(map_ + start_offset);
    finish_.set_node(start_.node_ + node_n);
}

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::reallocate_map_front(size_type add_num)
{
    size_type new_map_size = map_size_ + add_num;
    pointer *new_map = allocate_map(new_map_size);

    std::copy(map_, map_ + map_size_, new_map + add_num);

    const size_type start_offset = start_.node_ - map_;
    const size_type node_n = finish_.node_ - start_.node_;

    delete[] map_;

    map_size_ = new_map_size;
    map_ = new_map;

        // 最后添加分配好的 buffer
    for (size_type i = 0; i < add_num; ++i) {
        pointer temp = allocate_buffer();
        map_[i] = temp; // 加一个新的 buffer
    }

        // 设置新的控制中心
    start_.set_node(map_ + start_offset + add_num);
    finish_.set_node(start_.node_ + node_n);
}

template <typename Tp, typename Alloc>
template <typename... Args>
void deque<Tp, Alloc>::construct_back(Args &&...args)
{
    if (finish_.node_ == map_ + map_size_) reallocate_map_back(1);
    std::allocator_traits<allocator_type>::construct(allocator_, &(*(finish_++)), std::forward<Args>(args)...);
    ++sz_;
}

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::push_back(const value_type& val) { construct_back(val); }

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::push_back(value_type &&val) { construct_back(std::move(val)); }

template <typename Tp, typename Alloc>
template <typename... Args>
void deque<Tp, Alloc>::construct_front(Args &&...args)
{
    if (start_.node_ == map_) reallocate_map_front(1);
    std::allocator_traits<allocator_type>::construct(allocator_, &(*(--start_)), std::forward<Args>(args)...);
    ++sz_;
}

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::push_front(const value_type &val) { construct_front(val); }

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::push_front(value_type &&val) { construct_front(std::move(val)); }

template <typename Tp, typename Alloc>
void deque<Tp, Alloc>::printf_struct() const
{
    std::cout << "@printf_struct begin" << std::endl;
    std::cout << "sz_ : " << sz_ << ", " << "map_size_ : " << map_size_ << '\n';
    std::cout << "map - [ " << map_ << " ] \n";
    for (size_t i = 0; i < map_size_; ++i) {
        std::cout << "  buf - [ " << &map_[i] <<  " ] \n";
        for (size_t j = 0; j < MYSTL_DEQUE_BUFFER_SIZE; ++j) 
            std::cout << "    ele - [ "<< &map_[i][j] << " " << map_[i][j] <<" ] \n";
        std::cout << "    last - [ "<< (map_[i] + 4) << " ] \n";
    }

    std::cout << "map end - [ " << map_ + map_size_ << " ] \n";

    std::cout << "start - " << '\n';
    std::cout << "  cur   - " << start_.cur_ << '\n';
    std::cout << "  first - " << start_.first_ << '\n';
	std::cout << "  last  - " << start_.last_ << '\n';
	std::cout << "  node  - " << start_.node_ << '\n';

    std::cout << "finish - " << '\n';
    std::cout << "  cur   - " << finish_.cur_ << '\n';
    std::cout << "  first - " << finish_.first_ << '\n';
	std::cout << "  last  - " << finish_.last_ << '\n';
	std::cout << "  node  - " << finish_.node_ << '\n';
    std::cout << "@printf_struct end" << std::endl;
}

} // namespace mystl
