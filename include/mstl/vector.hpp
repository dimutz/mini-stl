#pragma once
#include <cstddef>

#include "allocator.hpp"
#include "utility.hpp"

namespace mstl {

template <typename T, typename Alloc = mstl::DefaultAllocator<T>>
class Vector {
   private:
	T* data_;				// pointer to allocated memory
	std::size_t size_;		// number of constructed elements
	std::size_t capacity_;	// allocated capacity
	Alloc alloc_;			// allocator instance

   public:
	// default constructor
	Vector() : data_(nullptr), size_(0), capacity_(0) {}

	// copy constructor
	Vector(const Vector& other)
		: data_(nullptr), size_(0), capacity_(0), alloc_() {
		reserve(other.size_);
		for (std::size_t i = 0; i < other.size_; i++)
			alloc_.construct(data_ + i, other.data_[i]);
		size_ = other.size_;
	}

	// move constructor
	Vector(Vector&& other) noexcept
		: data_(other.data_),
		  size_(other.size_),
		  capacity_(other.capacity_),
		  alloc_(mstl::move(other.alloc_)) {
		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	// destructor
	~Vector() {
		for (std::size_t i = 0; i < size_; i++)
			alloc_.destroy(data_ + i);
		alloc_.deallocate(data_, capacity_);
	}

	// copy assignment
	Vector& operator=(const Vector& other) {
		if (this == &other)
			return *this;
		for (std::size_t i = 0; i < size_; i++)
			alloc_.destroy(data_ + i);

		reserve(other.size_);

		for (std::size_t i = 0; i < other.size_; i++)
			alloc_.construct(data_ + i, other.data_[i]);

		size_ = other.size_;
		return *this;
	}

	// move assignment
	Vector& operator=(Vector&& other) noexcept {
		if (this == &other)
			return *this;

		for (std::size_t i = 0; i < size_; i++)
			alloc_.destroy(data_ + i);
		alloc_.deallocate(data_, capacity_);

		data_ = other.data_;
		size_ = other.size_;
		capacity_ = other.capacity_;
		alloc_ = mstl::move(other.alloc_);

		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;

		return *this;
	}

	// --------------------
	//    Element access
	// --------------------

	T& at(std::size_t pos) {
		if (pos >= size_)
			throw std::out_of_range("Vector::at() index out of range");
		return data_[pos];
	}

	const T& at(std::size_t pos) const {
		if (pos >= size_)
			throw std::out_of_range("Vector::at() index out of range");
		return data_[pos];
	}

	T& operator[](std::size_t pos) { return data_[pos]; }

	const T& operator[](std::size_t pos) const { return data_[pos]; }

	T& front() {
		if (size_ == 0)
			throw std::out_of_range("Vector::front() called on empty vector");
		return data_[0];
	}

	const T& front() const {
		if (size_ == 0)
			throw std::out_of_range("Vector::front() called on empty vector");
		return data_[0];
	}

	T& back() {
		if (size_ == 0)
			throw std::out_of_range("Vector::back() called on empty vector");
		return data_[size_ - 1];
	}

	const T& back() const {
		if (size_ == 0)
			throw std::out_of_range("Vector::back() called on empty vector");
		return data_[size_ - 1];
	}

	T* data() { return data_; }

	const T* data() const { return data_; }

	// --------------------
	//      Iterators
	// --------------------

	T* begin() { return data_; }

	const T* begin() const { return data_; }

	T* end() { return data_ + size_; }

	const T* end() const { return data_ + size_; }

	// --------------------
	//      Capacity
	// --------------------

	bool empty() const { return size_ == 0; }

	std::size_t size() const { return size_; }

	void reserve(std::size_t new_cap) {
		if (new_cap <= capacity_)
			return;

		T* new_data = alloc_.allocate(new_cap);

		// move old elements
		for (std::size_t i = 0; i < size_; i++) {
			alloc_.construct(new_data + i, mstl::move(data_[i]));
			alloc_.destroy(data_ + i);
		}

		// deallocate old memory
		alloc_.deallocate(data_, capacity_);

		// update pointers
		data_ = new_data;
		capacity_ = new_cap;
	}

	std::size_t capacity() const { return capacity_; }

	void shrink_to_fit() {
		if (size_ == capacity_)
			return;

		T* new_data = alloc_.allocate(size_);

		// move old elements
		for (std::size_t i = 0; i < size_; i++) {
			alloc_.construct(new_data + i, mstl::move(data_[i]));
			alloc_.destroy(data_ + i);
		}

		// deallocate old memory
		alloc_.deallocate(data_, capacity_);

		// update pointers
		data_ = new_data;
		capacity_ = size_;
	}

	// --------------------
	//      Modifiers
	// --------------------

	void clear() {
		for (std::size_t i = 0; i < size_; i++)
			alloc_.destroy(data_ + i);
		size_ = 0;
	}

	T* insert(T* pos, const T& value) {
		std::size_t idx = pos - data_;
		if (idx > size_)
			throw std::out_of_range("Vector::insert() iterator out of range");

		if (size_ == capacity_) {
			// double capacity
			std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			T* new_data = alloc_.allocate(new_capacity);

			// move old elements before pos
			for (std::size_t i = 0; i < idx; i++)
				alloc_.construct(new_data + i, data_[i]);

			// construct new element at pos
			alloc_.construct(new_data + idx, value);

			// move old elements after pos
			for (std::size_t i = idx; i < size_; i++)
				alloc_.construct(new_data + i, data_[i]);

			// destroy old elements
			for (std::size_t i = 0; i < size_; i++)
				alloc_.destroy(data_ + i);

			// deallocate old memory
			alloc_.deallocate(data_, size_);

			// update pointers
			data_ = new_data;
			capacity_ = new_capacity;
		} else {
			// shift elements to the right
			for (std::size_t i = size_; i > idx; i--) {
				alloc_.construct(data_ + i, mstl::move(data_[i - 1]));
				alloc_.destroy(data_ + i - 1);
			}

			// construct new element
			alloc_.construct(data_ + idx, value);
		}

		size_++;
		return data_ + idx;
	}

	T* insert(T* pos, T&& value) {
		std::size_t idx = pos - data_;
		if (idx > size_)
			throw std::out_of_range("Vector::insert() iterator out of range");

		if (size_ == capacity_) {
			// double capacity
			std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			T* new_data = alloc_.allocate(new_capacity);

			// move old elements before pos
			for (std::size_t i = 0; i < idx; i++)
				alloc_.construct(new_data + i, mstl::move(data_[i]));

			// construct new element at pos
			alloc_.construct(new_data + idx, mstl::move(value));

			// move old elements after pos
			for (std::size_t i = idx; i < size_; i++)
				alloc_.construct(new_data + i, mstl::move(data_[i]));

			// destroy old elements
			for (std::size_t i = 0; i < size_; i++)
				alloc_.destroy(data_ + i);

			// deallocate old memory
			alloc_.deallocate(data_, size_);

			// update pointers
			data_ = new_data;
			capacity_ = new_capacity;
		} else {
			// shift elements to the right
			for (std::size_t i = size_; i > idx; i--) {
				alloc_.construct(data_ + i, mstl::move(data_[i - 1]));
				alloc_.destroy(data_ + i - 1);
			}

			// construct new element
			alloc_.construct(data_ + idx, mstl::move(value));
		}

		size_++;
		return data_ + idx;
	}

	T* erase(T* pos) {
		std::size_t idx = pos - data_;
		if (idx >= size_)
			throw std::out_of_range("Vector::erase() out of range");

		alloc_.destroy(data_ + idx);

		// shift elements to the left
		for (std::size_t i = idx; i < size_ - 1; i++) {
			alloc_.construct(data_ + i, mstl::move(data_[i + 1]));
			alloc_.destroy(data_ + i + 1);
		}

		size_--;
		return data_ + idx;
	}

	void push_back(const T& value) { emplace_back(value); }

	void push_back(T&& value) { emplace_back(mstl::move(value)); }

	template <typename... Args>
	void emplace_back(Args&&... args) {
		if (size_ == capacity_) {
			// double capacity
			std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			T* new_data = alloc_.allocate(new_capacity);

			// move old elements
			for (std::size_t i = 0; i < size_; i++) {
				alloc_.construct(new_data + i, mstl::move(data_[i]));
				alloc_.destroy(data_ + i);
			}

			// deallocate old memory
			alloc_.deallocate(data_, capacity_);

			// update pointers
			data_ = new_data;
			capacity_ = new_capacity;
		}

		// construct new element in-place
		alloc_.construct(data_ + size_, mstl::forward<Args>(args)...);
		size_++;
	}

	void pop_back() {
		if (size_ == 0)
			throw std::out_of_range(
				"Vector::pop_back() called on empty vector");
		size_--;
		alloc_.destroy(data_ + size_);
	}

	void resize(std::size_t count) {
		if (count < size_) {
			for (std::size_t i = count; i < size_; i++)
				alloc_.destroy(data_ + i);
		} else if (count > size_) {
			reserve(count);
			for (std::size_t i = size_; i < count; i++)
				alloc_.construct(data_ + i);
		}
		size_ = count;
	}
};

}  // namespace mstl
