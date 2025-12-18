#pragma once 

#include <atomic>

class Base {
    std::atomic<int> counter_{0};
public:
    virtual ~Base() = default;
    void increment() { counter_.fetch_add(1, std::memory_order_relaxed); }
    int get() const { return counter_.load(); }
    virtual std::string name() const { return "Base"; }
};

class Derived1 : public Base {
    int id_;
public:
    explicit Derived1(int id) : id_(id) {
        std::cout << "Derived1(" << id << ") created\n";
    }
    ~Derived1() {
        std::cout << "Derived1(" << id_ << ") destroyed\n";
        id_ = -1;
    }
    std::string name() const override { 
        return "Derived1#" + std::to_string(id_); 
    }
};

class Derived2 : public Base {
    int id_;
public:
    explicit Derived2(int id) : id_(id) {
        std::cout << "Derived2(" << id << ") created\n";
    }
    ~Derived2() {
        std::cout << "Derived2(" << id_ << ") destroyed\n";
    }
    std::string name() const override { 
        return "Derived2#" + std::to_string(id_); 
    }
};

class TestClass
{
public:
    int getA() { return 1; }

    void setA(int num) {  }

private:

};