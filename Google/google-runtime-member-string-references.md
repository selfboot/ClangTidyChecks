```c++
#include <iostream>

class Foo {
public:
    Foo(const std::string cnt_str, std::string str): const_str(cnt_str), str(str) {}
    const std::string &const_str;       // 1.
    std::string &str;                   // 2.
};

int main(){
    Foo instance("const_string", "string");
    std::cout << instance.const_str << std::endl << instance.str << std::endl;      // 3
}
```

Foo 的成员变量 const_str 和 str 都是在成员初始化列表中通过栈中变量来赋初值的。main函数中，创建Foo时使用了字符串字面值，构造函数调用完毕后参数会立即销毁，因此foo对象的两个成员变量引用的地址就会无效，这时候代码3来获取成员变量时就会导致 `undefined behavior`。

不过 Clang-tidy 目前只检测成员变量中 std::string 的常量引用，所以2处的代码，clang-tidy 并不会报警。

## Dangling pointer or reference

当指针（引用）所指向的对象被释放或者收回时，该指针称为悬垂指针（引用）。

> A dangling pointer is a pointer to storage that is no longer allocated. Dangling pointers are nasty bugs because they seldom crash the program until long after they have been created, which makes them hard to find.

下面看几个悬挂指针的示例：

```c++
#include <iostream>

class Foo {
public:
    Foo(std::string cnt_str): const_str(cnt_str){}
    std::string &const_str;       // 1.
};

int *func()
{
    // x is local variable and goes out of scope after an execution of fun() is over.
    int x = 5;
    return &x;
}

int main(){
    // Dangling Pointers Sample
    Foo *object = new Foo("1");
    Foo *object2 = object;
    delete object;          // 1
    // object = nullptr;
    std::cout << object2->const_str << std::endl;

    int *p = func();        // 2
    std::cout << *p << std::endl;
    return 0;
}
```

## Ref

[What happens in C++ when I pass an object by reference and it goes out of scope?](https://stackoverflow.com/questions/7603833/what-happens-in-c-when-i-pass-an-object-by-reference-and-it-goes-out-of-scope)   
[Does a const reference prolong the life of a temporary?](https://stackoverflow.com/questions/2784262/does-a-const-reference-prolong-the-life-of-a-temporary)  
[Can a local variable's memory be accessed outside its scope?](https://stackoverflow.com/questions/6441218/can-a-local-variables-memory-be-accessed-outside-its-scope/6445794#6445794)  
[Should I prefer pointers or references in member data?](https://stackoverflow.com/questions/892133/should-i-prefer-pointers-or-references-in-member-data)  
[What is a dangling pointer](https://stackoverflow.com/questions/17997228/what-is-a-dangling-pointer)  


