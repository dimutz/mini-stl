#include <iostream>
#include <stdexcept>
#include <string>

#include "mstl/vector.hpp"

int main() {
	std::cout << "Testing mstl::DefaultAllocator...\n";
	{
		mstl::DefaultAllocator<std::size_t> alloc;

		std::size_t* p = alloc.allocate(5);
		for (std::size_t i = 0; i < 5; i++)
			alloc.construct(p + i, i * 10);
		std::cout << "std::size_tegers: ";
		for (std::size_t i = 0; i < 5; i++)
			std::cout << p[i] << " ";
		std::cout << std::endl;
		for (std::size_t i = 0; i < 5; i++)
			alloc.destroy(p + i);
		alloc.deallocate(p, 5);
	}
	{
		mstl::DefaultAllocator<std::string> alloc;

		std::string* p = alloc.allocate(3);

		alloc.construct(p + 0, "hello");
		alloc.construct(p + 1, "mini");
		alloc.construct(p + 2, "stl");

		std::cout << "Strings: ";
		std::cout << p[0] << " " << p[1] << " " << p[2] << "\n";

		alloc.destroy(p + 0);
		alloc.destroy(p + 1);
		alloc.destroy(p + 2);

		alloc.deallocate(p, 3);
	}

	std::cout << "Testing mstl::Vector...\n";

	// Basic push_back / emplace_back

	mstl::Vector<std::size_t> vi;

	std::cout << std::boolalpha << vi.empty() << std::endl;

	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	vi.emplace_back(4);
	vi.emplace_back(5);

	std::cout << "vi contents after push/emplace_back: ";
	for (auto x : vi)
		std::cout << x << " ";
	std::cout << "\n";

	// Element access

	try {
		std::cout << "vi.at(2): " << vi.at(2) << "\n";		// 3
		std::cout << "vi.front(): " << vi.front() << "\n";	// 1
		std::cout << "vi.back(): " << vi.back() << "\n";	// 5
	} catch (const std::out_of_range& e) {
		std::cout << e.what() << "\n";
	}

	// Iterators

	std::cout << "vi using iterators: ";
	for (auto it = vi.begin(); it != vi.end(); ++it)
		std::cout << *it << " ";
	std::cout << "\n";

	// Copy constructor & assignment

	mstl::Vector<std::size_t> vi_copy(vi);
	mstl::Vector<std::size_t> vi_assign;
	vi_assign = vi;

	std::cout << "vi_copy: ";
	for (auto x : vi_copy)
		std::cout << x << " ";
	std::cout << "\n";

	std::cout << "vi_assign: ";
	for (auto x : vi_assign)
		std::cout << x << " ";
	std::cout << "\n";

	// Move constructor & assignment

	mstl::Vector<std::size_t> vi_move(std::move(vi_copy));
	mstl::Vector<std::size_t> vi_move_assign;
	vi_move_assign = std::move(vi_assign);

	std::cout << "vi_move (after move constructor): ";
	for (auto x : vi_move)
		std::cout << x << " ";
	std::cout << "\n";

	std::cout << "vi_copy (after move): size=" << vi_copy.size() << "\n";

	std::cout << "vi_move_assign (after move assignment): ";
	for (auto x : vi_move_assign)
		std::cout << x << " ";
	std::cout << "\n";

	std::cout << "vi_assign (after move): size=" << vi_assign.size() << "\n";

	// pop_back

	vi_move.pop_back();
	std::cout << "vi_move after pop_back: ";
	for (auto x : vi_move)
		std::cout << x << " ";
	std::cout << "\n";

	// insert

	vi_move.insert(vi_move.begin() + 1, 99);
	std::cout << "vi_move after insert 99 at pos 1: ";
	for (auto x : vi_move)
		std::cout << x << " ";
	std::cout << "\n";

	// erase

	vi_move.erase(vi_move.begin() + 2);
	std::cout << "vi_move after erase pos 2: ";
	for (auto x : vi_move)
		std::cout << x << " ";
	std::cout << "\n";

	// clear

	vi_move.clear();
	std::cout << "vi_move after clear, size=" << vi_move.size() << "\n";

	// resize

	vi_move_assign.resize(8);
	std::cout << "vi_move_assign after resize(8): ";
	for (auto x : vi_move_assign)
		std::cout << x << " ";
	std::cout << "\n";

	vi_move_assign.resize(3);
	std::cout << "vi_move_assign after resize(3): ";
	for (auto x : vi_move_assign)
		std::cout << x << " ";
	std::cout << "\n";

	// reserve / shrink_to_fit

	vi_move_assign.reserve(20);
	std::cout << "vi_move_assign capacity after reserve(20): "
			  << vi_move_assign.capacity() << "\n";
	vi_move_assign.shrink_to_fit();
	std::cout << "vi_move_assign capacity after shrink_to_fit: "
			  << vi_move_assign.capacity() << "\n";

	// Exception checks

	try {
		vi_move_assign.at(100);
	} catch (const std::out_of_range& e) {
		std::cout << "Caught exception as expected: " << e.what() << "\n";
	}

	try {
		mstl::Vector<std::size_t> empty_vec;
		empty_vec.pop_back();
	} catch (const std::out_of_range& e) {
		std::cout << "Caught exception as expected: " << e.what() << "\n";
	}
}