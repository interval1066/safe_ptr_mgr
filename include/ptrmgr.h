#pragma once

#include <memory>
#include <vector>
#include <stdexcept>

template <typename T>
class SafePointerManager {
private:
    std::vector<std::unique_ptr<T>> pointers;  // Holds all managed pointers.

public:
    // Add a new object to the manager, taking ownership of the pointer.
    void add(std::unique_ptr<T> ptr) {
        if (!ptr) {
            throw std::invalid_argument("Cannot add a null pointer.");
        }
        pointers.push_back(std::move(ptr));  // Move ownership into the vector.
    }

    // Get the raw pointer from the manager (note: this does not transfer ownership).
    T* get(size_t index) const {
        if (index >= pointers.size()) {
            throw std::out_of_range("Index out of range.");
        }
        return pointers[index].get();  // Return raw pointer, ownership is not transferred.
    }

    // Remove a pointer at the given index from the manager.
    void remove(size_t index) {
        if (index >= pointers.size()) {
            throw std::out_of_range("Index out of range.");
        }
        pointers.erase(pointers.begin() + index);  // Remove and destroy the unique_ptr.
    }

    // Get the total number of managed pointers.
    size_t size() const {
        return pointers.size();
    }

    // Move a pointer from another SafePointerManager to this one.
    void move(SafePointerManager<T>& other, size_t index) {
        if (index >= other.pointers.size()) {
            throw std::out_of_range("Index out of range.");
        }

        // Take ownership of the pointer from the other manager.
        this->add(std::move(other.pointers[index]));
        other.pointers.erase(other.pointers.begin() + index);  // Remove from the source manager.
    }

    // Reset the manager and release all managed pointers (destruct all objects).
    void reset() {
        pointers.clear();  // Clear vector and destroy the objects.
    }

    // Destructor: no need for explicit delete, as std::unique_ptr will clean up.
    ~SafePointerManager() = default;
};

