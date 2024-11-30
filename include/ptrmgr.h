#pragma once

#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>

// Base class for all pointer types in the pool
class PointerManagerBase {
public:
    virtual ~PointerManagerBase() = default;
    virtual void* get() = 0;
    virtual const std::type_info& getType() const = 0;
};

// Template class for handling specific pointer types
template <typename T>
class PointerManager : public PointerManagerBase {
private:
    std::unique_ptr<T> ptr;

public:
    PointerManager(T* raw_ptr) : ptr(raw_ptr) {}
    
    // Override to return raw pointer
    void* get() override {
        return static_cast<void*>(ptr.get());
    }

    // Override to return the type info
    const std::type_info& getType() const override {
        return typeid(T);
    }
};

// The main class that manages all pointers
class MixedPointerManager {
private:
    std::vector<std::unique_ptr<PointerManagerBase>> pool;

public:
    // Add a pointer of any type to the pool
    template <typename T>
    void addPointer(T* ptr) {
        pool.push_back(std::make_unique<PointerManager<T>>(ptr));
    }

    // Get the pointer at a particular index, safely cast to the required type
    template <typename T>
    T* getPointer(std::size_t index) {
        if (index < pool.size() && pool[index]->getType() == typeid(T)) {
            return static_cast<T*>(pool[index]->get());
        } else {
            std::cerr << "Invalid type or index!" << std::endl;
            return nullptr;
        }
    }

    // Print types of all stored pointers for debugging
    void printTypes() const {
        for (const auto& item : pool) {
            std::cout << "Stored pointer type: " << item->getType().name() << std::endl;
        }
    }
};


