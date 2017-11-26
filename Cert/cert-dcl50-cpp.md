# cert-dcl50-cpp (`C++11`)

```c++
#include <cstdarg>
 
int add(int first, int second, ...) {
  int r = first + second; 
  va_list va;
  va_start(va, second);
  while (int v = va_arg(va, int)) {
    r += v;
  }
  va_end(va);
  return r;
}
```

[DCL50-CPP. Do not define a C-style variadic function](https://www.securecoding.cert.org/confluence/display/cplusplus/DCL50-CPP.+Do+not+define+a+C-style+variadic+function)


## 可变参数

所谓可变参数指的是函数的参数个数可变，参数类型不定的函数。C中有一种特殊的省略符形参，可以用它传递可变数量的实参，不过这种一般只用于与C函数交互的接口程序。

实现原理 [【C++基础之二十】可变参数的函数](http://blog.csdn.net/jackystudio/article/details/17523523)  

C++11提供了两种主要的方法：如果所有的实参类型相同，可以传递一个名为initializer_list的标准库类型；如果实参的类型不同，我们可以编写可变参数模板。


[C++学习之可变参数的函数与模板](https://songlee24.github.io/2014/07/22/cpp-changeable-parameter/)  

