#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>
#include <string>
#include <shared_mutex>
#include <mutex>
#include <atomic>

namespace managed
{
	class MixedPointerManager {
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

		mutable std::shared_mutex mutex_;                       // Shared mutex for fine-grained locking
		std::vector<std::unique_ptr<BaseHolder>> objects_;      // Collection of objects
		std::atomic<std::size_t> size_{0};                      // Atomic size for fast read access

	public:
		MixedPointerManager() {
			objects_.reserve(16); // Preallocate memory for initial objects
		}

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
			auto holder = std::make_unique<Holder<T>>(std::forward<Args>(args)...);
			{
				std::unique_lock lock(mutex_);
            objects_.emplace_back(std::move(holder));
            ++size_; // Increment size atomically
			}
		}

		// Get an object by index and type
		template <typename T>
		T* get(std::size_t index) const {
			std::shared_lock lock(mutex_);
			if (index >= size_) { // Use atomic size_
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
			std::unique_lock lock(mutex_);
			if (index >= size_) {
				throw std::out_of_range("Index out of range.");
			}
			objects_.erase(objects_.begin() + index);
			--size_; // Decrement size atomically
		}

		// Clear all managed objects
		void clear() {
			std::unique_lock lock(mutex_);
			objects_.clear();
			size_ = 0; // Reset size atomically
		}

		// Get the number of managed objects
		std::size_t size() const noexcept {
			return size_.load(); // Atomic read
		}
	};
}

