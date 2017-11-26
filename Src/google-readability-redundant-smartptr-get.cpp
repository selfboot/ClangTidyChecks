#include <iostream>

struct Foo
{
    Foo(int in) : a(in) {}
    void show() const
    {
        std::cout << "a = " << a << '\n';
    }
    int a;
};

int main() {
    std::shared_ptr<int> i = std::shared_ptr<int>(new int(1));
    // 1.
    std::cout << *i.get() << std::endl;

    auto ptr = std::make_shared<Foo>(10);
    // 2.
    ptr.get()->show();
}

/*
std::cout << *i << std::endl;
ptr->show();
 */

