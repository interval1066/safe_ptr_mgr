#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>
#include <string>
#include <mutex>

class MixedPointerManager {
public:
    MixedPointerManager() = default;

    // Delete copy constructor and assignment operator
    MixedPointerManager(const MixedPointerManager&) = delete;
    MixedPointerManager& operator=(const MixedPointerManager&) = delete;

    // Move constructor
    MixedPointerManager(MixedPointerManager&& other) noexcept = default;

    // Move assignment operator
    MixedPointerManager& operator=(MixedPointerManager&& other) noexcept = default;

    // Destructor
    ~MixedPointerManager() = default;

    // Add an object of any type
    template <typename T, typename... Args>
    void add(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        objects_.emplace_back(std::make_unique<Holder<T>>(std::forward<Args>(args)...));
    }

    // Get an object by index and type
    template <typename T>
    T* get(std::size_t index) const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= objects_.size()) {
            throw std::out_of_range("Index out of range.");
        }
        auto* holder = dynamic_cast<Holder<T>*>(objects_[index].get());
        if (!holder) {
            throw std::bad_cast();
        }
        return &holder->value;
    }

    // Remove an object by index
    void remove(std::size_t index) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (index >= objects_.size()) {
            throw std::out_of_range("Index out of range.");
        }
        objects_.erase(objects_.begin() + index);
    }

    // Clear all managed objects
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        objects_.clear();
    }

    // Get the number of managed objects
    std::size_t size() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return objects_.size();
    }

private:
    // Base class for type erasure
    struct BaseHolder {
        virtual ~BaseHolder() = default;
    };

    // Derived holder to manage objects of any type
    template <typename T>
    struct Holder : BaseHolder {
        T value;

        explicit Holder(T&& val) : value(std::forward<T>(val)) {}
        template <typename... Args>
        explicit Holder(Args&&... args) : value(std::forward<Args>(args)...) {}
    };

    mutable std::mutex mutex_; // Mutex to synchronize access
    std::vector<std::unique_ptr<BaseHolder>> objects_; // Collection of objects
};

