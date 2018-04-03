


the copy constructor and copy assignment of std::exception shall be noexcept, and this is testable with:

```
static_assert(std::is_nothrow_copy_constructible<std::exception>::value, "");
static_assert(std::is_nothrow_copy_assignable<std::exception>::value, "");
```

[Should I declare the copy constructor of my exceptions noexcept?](https://stackoverflow.com/questions/21644735/should-i-declare-the-copy-constructor-of-my-exceptions-noexcept)  


[Exception and Copy Constructor : C++](https://stackoverflow.com/questions/16432959/exception-and-copy-constructor-c)  



First, when you write

```
throw Exception();
```

what's being thrown isn't actually the temporary object created by the prvalue expression Exception(). Conceptually, there's a separate object - the exception object - that's initialized from that temporary object, and it is the exception object that's actually thrown. (Compilers are allowed to elide the copy/move, though.)

Second, the language rules say that the exception object is always considered an lvalue. Hence it is allowed to bind to non-const lvalue references.

[c++ exceptions throw by value catch by reference](https://stackoverflow.com/questions/28932115/c-exceptions-throw-by-value-catch-by-reference)



# nothrow copy constructible


```
#include <iostream>

struct A { };
struct B { B(const B&){} };
struct C { C(const C&) noexcept {} };
struct D { int a;};
struct E { std::string a;};
struct F { F(const F&)= default; };
struct G { std::string a; G(const G&)= default; };

int main() {
    std::cout << std::boolalpha;
    std::cout << "is_nothrow_copy_constructible:" << std::endl;
    std::cout << "int: " << std::is_nothrow_copy_constructible<int>::value << std::endl;
    std::cout << "A: " << std::is_nothrow_copy_constructible<A>::value << std::endl;
    ...
    return 0;
}
```


[std::is_nothrow_copy_constructible](http://www.cplusplus.com/reference/type_traits/is_nothrow_copy_constructible/)    


# [copy elision](https://en.wikipedia.org/wiki/Copy_elision)



```c++
struct C {
    C() {}
    C(const C&) { std::cout << "A copy was made.\n"; }
};

C f() {
    return C();
}

int main() {
    std::cout << "Hello World!\n";
    C obj = f();
}
```

可以参考 [SO: What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization)  









