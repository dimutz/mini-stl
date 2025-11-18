#pragma once
#include <cstddef>
#include <new>

#include "utility.hpp"

namespace mstl {

template <typename T>
class DefaultAllocator {
   public:
	// allocate raw memory for n objects
	T* allocate(std::size_t n) {
		if (n == 0)
			return nullptr;
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	// deallocate raw memory
	void deallocate(T* ptr, size_t /*n*/) { ::operator delete(ptr); }

	// construct object at a memory location
	template <typename... Args>
	void construct(T* ptr, Args&&... args) {
		new (ptr) T(mstl::forward<Args>(args)...);
	}

	// destroy object at a memory location
	void destroy(T* ptr) { ptr->~T(); }
};

}  // namespace mstl
