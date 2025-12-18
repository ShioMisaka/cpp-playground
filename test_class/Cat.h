#pragma once

#include <string>
#include <iostream>

class Cat
{
public:
    Cat(): name_("") {}

    Cat(const std::string& name): name_(name), age_(0)
    {
        std::cout << "构造了" << name_ << std::endl;
    }

    Cat(const std::string& name, int age): name_(name), age_(age)
    {
        std::cout << "构造了" << name_ << std::endl;
    }

    ~Cat() {
        name_ = "";
        age_ = 0;
    }

    void Speak(const std::string& msg) const
    {
        std::cout << name_ << ": " << msg << std::endl;
    }

    std::string name() const { return name_; }

    int age() const { return age_; }

    bool operator<(const Cat& other) { return age_ < other.age_; }
    bool operator>(const Cat& other) { return age_ > other.age_; }
    bool operator<=(const Cat& other) { return age_ <= other.age_; }
    bool operator>=(const Cat& other) { return age_ >= other.age_; }
    bool operator==(const Cat& other) { return age_ == other.age_; }
    bool operator!=(const Cat& other) { return age_ == other.age_; }

private:
    std::string name_;

    int age_;
};

inline std::ostream& operator<<(std::ostream& os, const Cat& cat)
{
    os <<  "{" << cat.name() << "," << cat.age() << "}";
    return os;
}