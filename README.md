# SimpleVector

A minimal implementation of a dynamic array (`std::vector`-like) in modern C++, written from scratch as a learning project. This container supports dynamic resizing, direct element access, range-based for loops, and basic iterator operations.

## Features

✅ Templated `SimpleVector` class  
✅ Basic construction:
- Default constructor
- Constructor with size and optional fill value
- `std::initializer_list` constructor  
  ✅ Element access:
- `operator[]` with `assert`
- `At()` with bounds checking  
  ✅ Size and capacity methods:
- `GetSize()`, `GetCapacity()`, `IsEmpty()`  
  ✅ Modifying methods:
- `Resize()` — changes the number of elements
- `Clear()` — resets the size to zero  
  ✅ Iterators:
- `begin()`, `end()`, `cbegin()`, `cend()`  
  ✅ Works with range-based for loops  
  ✅ Uses `ArrayPtr<Type>` as internal storage helper

## Example Usage

```cpp
SimpleVector<int> vec(3, 42);  // vector of size 3 filled with 42

for (int val : vec) {
    std::cout << val << " ";
}
// Output: 42 42 42

vec.Resize(5);  // adds two default-initialized elements (0 for int)

std::cout << "Size: " << vec.GetSize();  // Output: Size: 5
```
## Build & Run

g++ -std=c++20 -Wall -Wextra -pedantic main.cpp -o simple_vector
./simple_vector

## License

This project is open-source and available under the MIT License.
