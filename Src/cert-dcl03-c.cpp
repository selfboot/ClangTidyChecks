#include <iostream>
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

void print_number(const int* myInt) {
    assert(myInt!=nullptr);                         // Assert in runtime
    std::cout << *myInt << std::endl;
}

void check_env(){
    int i;
    assert(sizeof(void *) >= sizeof i);             // Assert in compile time
    // static_assert(sizeof(void *) >= sizeof i);   // Better using static_assert
}

int main ()
{
    int a=10;
    int *b = &a;
    int *c = nullptr;

    print_number(b);
    print_number(c);

    check_env();
    return 0;
}
