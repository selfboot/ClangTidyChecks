#include <iostream>

class Foo
{
public:
    // 1. single parameter constructor, can be used as an implicit conversion
    Foo (int foo) : m_foo (foo)
    {
    }

    int GetFoo () { return m_foo; }
    // 2. type conversion operator, can be used as an implicit conversion
    operator bool() const { return true; }

private:
    int m_foo;
};

void DoBar (Foo foo)
{
    int i = foo.GetFoo();
    std::cout<< i << std::endl;
}

int main() {
    DoBar (42);

    Foo a{1};
    Foo b{2};
    std::cout<< (a == b) << std::endl;
}

/*
42
1
*/
