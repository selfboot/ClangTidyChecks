#include <iostream>

class Foo {
public:
    Foo(const std::string cnt_str, std::string str): const_str(cnt_str), str(str) {}
    const std::string &const_str;       // 1.
    std::string &str;                   // 2.
};

int *func()
{
    // x is local variable and goes out of scope after an execution of fun() is over.
    int x = 5;
    return &x;
}

int main(){
    Foo instance("const_string", "strings");
    std::cout << instance.const_str << std::endl << instance.str << std::endl;

    // Dangling Pointers Sample
    Foo *object = new Foo("1", "2");
    Foo *object2 = object;
    delete object;          // 1
    // object = nullptr;
    std::cout << object->const_str << std::endl << instance.str << std::endl;

    int *p = func();        // 2
    std::cout << *p << std::endl;
    return 0;
}
