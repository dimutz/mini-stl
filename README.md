# mini-stl

mini-stl is a header-only C++ library implementing core STL containers and algorithms.  
It’s a learning project to deeper understand how STL works under the hood.

## Features

- `Vector` – dynamic array with iterators  
- `List` – doubly-linked list  
- `Array` – fixed-size array  
- `Map` – key-value store using Red-Black Tree  
- Basic algorithms: `copy`, `fill`, `find`, `sort`  
- Custom memory allocator  

## Project Structure

- include/mstl/ : Library headers
- test/ : Unit tests
- benchmark/ : Benchmarks
- main.cpp : Demo usage
- CMakeLists.txt


## Build & Run

```bash
mkdir build && cd build
cmake ..
make

./mini_stl      # demo
./tests         # run tests
./bench         # run benchmarks
```

License

mini-stl is under MIT License.
