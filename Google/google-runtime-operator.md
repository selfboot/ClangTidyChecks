```c++
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
    // 2. no matching function for call to 'show'
    // show(&demo);
}
```

上面代码中，Foo类重载了一元操作符&，clang-tidy会检测这种情况。google 规范建议不要重载一元操作符&（[Google：Operator Overloading](https://google.github.io/styleguide/cppguide.html#Operator_Overloading)），因为&是C++中默认的取值操作符，有着基础的语义，重载可能会导致问题，上面2处“正常”的代码就无法通过编译。（[Overloading unary operator &](https://stackoverflow.com/questions/6410333/overloading-unary-operator) ）
 
不过特殊情况中，也可以重载&，比如实现了一个封装指针的对象，想将所有对该对象的操作转发到封装的指针上（[What legitimate reasons exist to overload the unary operator&?](https://stackoverflow.com/questions/6495977/what-legitimate-reasons-exist-to-overload-the-unary-operator)）。


