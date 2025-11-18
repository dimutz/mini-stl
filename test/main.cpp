#include <iostream>
#include <string>

#include "mstl/allocator.hpp"

int main() {
	std::cout << "Testing mstl::DefaultAllocator...\n";
	{
		mstl::DefaultAllocator<int> alloc;

		int* p = alloc.allocate(5);
		for (int i = 0; i < 5; i++)
			alloc.construct(p + i, i * 10);
		std::cout << "Integers: ";
		for (int i = 0; i < 5; i++)
			std::cout << p[i] << " ";
		std::cout << std::endl;
		for (int i = 0; i < 5; i++)
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

	return 0;
}