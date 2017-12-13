```c++
#include <iostream>

void throw_by_pointer() {
    int num = 2;
    int *num_ptr = &num;
    std::cout << num_ptr << std::endl;
    std::cout << *num_ptr << std::endl;
    throw num_ptr;              // Throw a pointer is dangerous.
}

class ClassA {
public:
    virtual void PrintMe() const {
        std::cout << "ClassA" << std::endl;
    }
};

class ClassB : public ClassA {
public:
    void PrintMe() const override {
        std::cout << "ClassB" << std::endl;
    }
};

void throw_by_value() {
    auto *obj = new ClassB;
    throw *obj;
}

int main() {
    try {
        throw_by_pointer();
    } catch(const int *p) {
        std::cout << p << std::endl;
        std::cout << *p << std::endl;
    }

    try {
        throw_by_value();
    }
    catch(const ClassA a) {
        //  Catching by value results in slicing at the catch site
        a.PrintMe();
    }

    return 0;
}
```

上面代码中 throw_by_pointer() 函数抛出了一个指向栈中内容的指针，在catch时其指向的内容已经被销毁，因此catch中解引用该指针将导致未定义行为。 throw_by_value() 函数抛出了一个派生类的值，可以catch基类的值或者引用，如果catch基类的值，则会丢失派生类的信息。catch基类引用则可以成功保留派生类的信息，可以正常使用多态属性。

[C++ Coding Standards Item 73](http://ptgmedia.pearsoncmg.com/images/0321113586/items/sutter_item73.pdf) 中对于异常有以下建议.   

> Throw by value, catch by reference.   

# Throw by value

C++中抛出异常时，throw 一个对象的值而不是指针。如果 throw 指针，则需要处理内存管理的问题，如果抛出一个指向栈分配值的指针，则在catch时该指针指向的内容已经被析构。当然，可以抛出指向堆内存的指针，不过这样就必须在catch中必须手动地释放掉堆内存。如果一定要抛出指针，则建议使用智能指针，避免自己管理内存。

直接throw对象的值则不会有上面的问题，我们唯一要做的就是**保证抛出对象的拷贝构造函数不抛异常**，详细可以参考 [cert-err60-cpp](./cert-err60-cpp.md)。

此外，值的注意的是，throw语句抛出对象的类型是编译期决定的，为抛出语句中该对象的静态类型。如下代码中，catch 语句会匹配 base类型，而不是derived。详细可以参考 [SO: C++ throw dereferenced pointer](https://stackoverflow.com/questions/7014626/c-throw-dereferenced-pointer) 

```c++
#include <iostream>
struct base { };
struct derived : base { };

void t() {
    derived d;
    base *b = &d;
    throw *b;
}

int main() {
    try {
        t();
    } catch(derived const &) {
        std::cout << "derived" << std::endl;
    } catch(base const &) {
        std::cout << "base" << std::endl;
    }
}
```

在抛出异常时，最好是抛出一个匿名临时对象。具体原因可以参考 [ERR09-CPP. Throw anonymous temporaries](https://wiki.sei.cmu.edu/confluence/display/cplusplus/ERR09-CPP.+Throw+anonymous+temporaries)  

# Catch by reference

如果抛出智能指针，则可以捕获指针对象本身（指针保留了多态特性）。如果抛出的是值，则必须catch一个引用，这样才可以保留多态，不然就会发生截断。可以阅读 [SO: C++ catch blocks - catch exception by value or reference?](https://stackoverflow.com/questions/2522299/c-catch-blocks-catch-exception-by-value-or-reference)  

对于开始的代码，如果catch by reference，则调用 ClassB 的 PrintMe 函数，如下：

```c++
try {
    throw_by_value();
}
catch(const ClassA &a) {
    // Catching by reference preserves the polymorphism of the exception object.
    a.PrintMe();        // ClassB
}
``` 

# Exceptions

Clang-Tidy 会检查到大部分不遵循 `Throw by value, catch by reference` 的情况，不过对于下面的的情况，并不会检测。

1. 抛出字符串字面值的情况。尽管这里抛出的实际上是一个指针，但是它不会被截断，另外大家也习惯使用字符串字面值；
2. Catch 字符指针(char, wchar_t, unicode character types)的情况，抛出字符串字面值时需要用char*来catch；

该检查项默认开启了选项`CheckThrowTemporaries`，会检测出违反 `Throw anonymous temporaries` 的情况，不过并不会标记下面的情况。

1. 抛出 std::move 获取到的值。`std::string a("DEMO"); throw std::move(a);`
2. 抛出函数参数的情况。这样就可以写抛异常的 helper 函数。
3. 在catch语句中重新抛出捕获到的异常。不过这种情况，一般只用写 `throw;` 即可。

