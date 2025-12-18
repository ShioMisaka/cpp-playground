#pragma once

#include <iostream>

#include "Cat.h"

class Entity
{
public:
    Entity(): index(-1), pCat(nullptr) {
        std::cout << "Entity 默认构造: " << index <<  std::endl;
    }

    ~Entity() {
        std::cout << "Entity 析构函数: " << index << std::endl;
        if (pCat != nullptr)
            delete pCat;
    }


    explicit Entity(int ind): index(ind), pCat(nullptr) {
        std::cout << "Entity 有参构造: " << index <<  std::endl;
    }

    Entity(const Entity& other) : index(other.index), pCat(nullptr) {
        std::cout << "Entity 拷贝构造: " << index <<  std::endl;
        if (other.pCat != nullptr)
            pCat = new Cat(other.pCat->name());
    }

    Entity(Entity&& other) noexcept : index(other.index), pCat(other.pCat) {
        std::cout << "Entity 移动构造: "  << index <<  std::endl;
        other.index = -1;
        other.pCat = nullptr;
    }

    Entity& operator=(const Entity& other) {
        std::cout << "Entity 拷贝赋值: " << other.index << std::endl;
        if (this != &other) {
            index = other.index;
            if (pCat != nullptr)
                delete pCat;
            if (other.pCat != nullptr)
                pCat = new Cat(other.pCat->name());
        }
        return *this;
    }

    Entity& operator=(Entity&& other) noexcept {
        std::cout << "Entity 移动赋值: " << other.index << std::endl;
        if (this != &other) {
            index = other.index;
            pCat = other.pCat;
            other.index = 0;
            other.pCat = nullptr;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Entity &e);

private:
    int index;

    Cat *pCat;
};

inline std::ostream& operator<<(std::ostream& os, const Entity& e)
{
    os << "Enitity: " << e.index;
    return os;
}