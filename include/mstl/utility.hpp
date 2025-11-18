#pragma once
#include <cstddef>
#include <type_traits>

namespace mstl {

// pair<T1, T2>
template <typename T1, typename T2>
struct pair {
	T1 first;
	T2 second;

	pair() = default;
	pair(const T1& a, const T2& b) : first(a), second(b) {}
	pair(T1&& a, T2&& b) : first(std::move(a)), second(std::move(b)) {}
};

// swap
template <typename T>
void swap(T& a, T& b) {
	T temp = std::move(a);
	a = std::move(b);
	b = std::move(temp);
}

// move
template <typename T>
constexpr T&& move(T& arg) noexcept {
	return static_cast<T&&>(arg);
}

// forward
template <typename T>
constexpr T&& forward(std::remove_reference_t<T>& arg) noexcept {
	return static_cast<T&&>(arg);
}

template <typename T>
constexpr T&& forward(std::remove_reference_t<T>&& arg) noexcept {
	static_assert(!std::is_lvalue_reference<T>::value);
	return static_cast<T&&>(arg);
}

}  // namespace mstl
