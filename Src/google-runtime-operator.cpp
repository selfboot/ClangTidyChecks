#include <iostream>

class Foo
{
public:
    explicit Foo(int data=2): data(data){};
    // 1. Overload unary operator &.
    int* operator &()
    {
        return &data;
    }
    void echo(){
        std::cout << data << std::endl;
    }
private:
    int data;
};

void show(Foo *foo)
{
    foo->echo();
}

int main(){
    Foo demo;
    // no matching function for call to 'show'
    // show(&demo);
}


