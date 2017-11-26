# cert-dcl21-cpp

```c++
#include <iostream>

class Foo {
    int data=0;
 public:
    Foo operator++(int) {
        Foo R(*this);
        data++;
        return R;
    }
    int getData() const { return data; }
};

int main() {
    Foo c;
    c++++;                      // Should not be allowable.
    std::cout << c.getData();   // 1
}
```

详细解释可以参考 [DCL21-CPP. Overloaded postfix increment and decrement operators should return a const object](https://www.securecoding.cert.org/confluence/display/cplusplus/DCL21-CPP.+Overloaded+postfix+increment+and+decrement+operators+should+return+a+const+object)。

## prefix, postfix increment(decrement)

为了区分++和--操作符的前缀与后缀调用，C++语言规定后缀调用的函数原型有一个int类型参数，当函数被调用时，编译器传递一个0做为int参数的值给该函数。这两个操作符号的原型如下：

```c++
class X {
public:
  X& operator++();   // prefix ++a
  const X operator++(int); // postfix a++
};
```

需要特别注意的是：这些操作符前缀与后缀形式返回值类型是不同的。`前缀形式返回一个引用，后缀形式返回一个const类型`。下面讨论++操作符的前缀与后缀形式，这些说明也同样适用于--操作符。


http://www.tk4479.net/janeqi1987/article/details/75981927

可以阅读：More Effective C++: Item 6


