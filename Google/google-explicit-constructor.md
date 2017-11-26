```c++
#include <iostream>

class Foo
{
public:
    // 1. single parameter constructor， can be used as an implicit conversion
    Foo (int foo) : m_foo (foo)
    {
    }

    int GetFoo () { return m_foo; }
    // 2. type conversion operator， can be used as an implicit conversion
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
```

上面 main 函数中， DoBar 传入 int 型参数仍然调用了 void DoBar (Foo foo)，此外， 还对 Foo 类型对象使用 == 进行比较。之所以能够进行上面的操作，是因为发生了**隐式类型转换**。有时候，这里的隐式类型转换可能不是我们的本意，导致奇怪的现象。

Google 规范认为（[Google: Implicit Conversions](https://google.github.io/styleguide/cppguide.html#Explicit_Constructors)）：

> Type conversion operators， and constructors that are callable with a single argument， must be marked explicit in the class definition. As an exception， copy and move constructors should not be explicit， since they do not perform type conversion. 

## Implicit Conversions

C++的基本类型中并非完全的对立，部分数据类型之间是可以进行隐式转换的。所谓隐式转换，是指不需要用户干预，编译器私下进行的类型转换行为，很多时候用户可能都不知道进行了哪些转换。

C++面向对象的多态特性，就是通过父类的类型实现对子类的封装。通过隐式转换，你可以直接将一个子类的对象使用父类的类型进行返回。在比如，数值和布尔类型的转换，整数和浮点数的转换等。某些方面来说，隐式转换给C++程序开发者带来了不小的便捷。（[类型转换和类型安全（现代 C++）](https://msdn.microsoft.com/zh-cn/library/hh279667.aspx)）

对于基本数据类型，转换以取值范围的作为转换基础（保证精度不丢失）。隐式转换发生在从小->大的转换中。比如从char转换为int。
从int-》long。对于自定义对象，子类对象可以隐式的转换为父类对象。

隐式类型转换([C++隐式类型转换](http://www.cnblogs.com/solidblog/p/3381628.html))通常发生在以下几种情况：

1. 混合类型的算术运算表达式中。

    ```
    int a = 3;
    double b = 4.5;
    a + b; // a将会被自动转换为double类型，转换的结果和b进行加法操作
    ```

2. 不同类型的赋值操作。

    ```
    int a = true;  // bool类型被转换为int类型
    int * ptr = null; // null被转换为int*类型
    ```

3. 函数参数传值。

    ```
    void func(double a);
    func(1); // 1被隐式的转换为double类型1.0
    ```

4. 函数返回值。

    ```
    double add(int a， int b)
    {
        return a + b;
    } //运算的结果会被隐式的转换为double类型返回
    ```

以上四种情况下的隐式转换，都满足了一个基本原则：低精度 –》 高精度转换。在有转换运算符（type conversion operators）和单参数构造函数（single-argument constructors）的地方，隐式转换可能会导致意想不到的结果。

## single-argument constructors

按照默认规定，**只有一个参数的构造函数也定义了一个隐式转换，将该构造函数对应数据类型的数据转换为该类对象**。如下面所示：

```c++

#include <iostream>

class Foo {
public:
    Foo(int n) {
        size = n;
    };

    Foo(const char* p) {
        data = p;
    };

    int size;
    std::string data;
};

int main() {
    Foo foo1(10);       //1. OK
    Foo foo2 = 10;      //2. 编译通过, 和1一样

    Foo foo3("foo3");   //3. OK
    Foo foo4 = "foo4";  //4. 编译通过，和3一样
    std::cout << foo1.size << ", " << foo2.size << ", "
              << foo3.data << ", " << foo4.data << std::endl;
}
```

foo2 和 foo4 分别把一个int型和char*型隐式转换成了Foo，容易导致问题。C++11 提供了`explicit关键字`，在构造函数声明的时候加上explicit关键字，能够禁止隐式转换。注意explicit关键字只能用于类内部的构造函数声明上，而不能用在类外部的函数定义上。google的c++规范中提到explicit的优点是可以避免不合时宜的类型变换，缺点无。所以google约定所有单参数的构造函数都必须是显示的，只有极少数情况下拷贝构造函数可以不声明称explicit。([What does the explicit keyword mean?](https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean))

## type conversion operator 

类型转换操作符（[CPPreference: user-defined conversion](http://en.cppreference.com/w/cpp/language/cast_operator)）是一种特殊的类成员函数，它定义将类类型值转变为其他类型值的转换。转换操作符在类定义体内声明，在保留字 operator 之后跟着转换的目标类型。

```c++
#include <iostream>

struct Foo {
    //implicit conversion
    operator int() const { return 7; }

    // explicit conversion
    explicit operator int *() const { return nullptr; }
};

int main() {
    Foo x;

    int n = static_cast<int>(x);   // OK: sets n to 7
    int m = x;                     // OK: sets m to 7
    std::cout << m << ", " << n << std::endl;
    int *p = static_cast<int *>(x);  // OK: sets p to null
    //  int* q = x; // Error: no implicit conversion
}

```

类型转换函数必须是成员函数，不能指定返回类型，并且形参表必须为空；返回值是隐含的，返回值是与转换的类型相同的。存在类型转换操作符的类中，有可能发生意料之外的隐式类型转换，和单参数构造函数的情况类似。因此需要使用 explicit 关键字来强制不进行隐式类型转换([C++ 中使用explicit关键字避免隐式转换](http://www.cnblogs.com/likebeta/archive/2012/07/31/explicit.html))。


