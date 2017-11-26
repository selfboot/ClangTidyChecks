#include <cstdio>
#include <cstdlib>
#include <iostream>

// replacement of a minimal set of functions:
void* operator new(std::size_t sz) {
    std::cout << "global op new called, size = %zu\n" << sz << std::endl;
    return std::malloc(sz);
}
void operator delete(void* ptr) noexcept
{
    std::cout << "global op delete called" << std::endl;
    std::free(ptr);
}
int main() {
    auto* p1 = new int;
    delete p1;

    auto* p2 = new int[10]; // guaranteed to call the replacement in C++11
    delete[] p2;
}
