```c++
#include <iostream>

// 1. parameter as reference
void foo(std::string &in, std::string *out){
    *out = in;
}

int main(){
    std::string in = "src";
    std::string out;
    foo(in, &out);

    std::cout << out << std::endl;
    return 0;
}
```

在C中，为了改变参数的值，我们会传入指针参数，比如 `int foo(int *pval)`。C++中多了一种方法，就是传入引用，比如`int foo(int &val)`。不过引用有以下问题：

> References can be confusing, as they have value syntax but pointer semantics.

Google规范（[Google: Reference Arguments](https://google.github.io/styleguide/cppguide.html#Reference_Arguments)）认为只读参数（输入参数）为值或者const reference，或者const pointers，值改动的参数（输出参数）只能为指针类型。

## pass by value, const reference or const pointer

C++中为了避免函数调用者改变参数的值，有如下三种方法（[Const Correctness](https://isocpp.org/wiki/faq/const-correctness#const-ref-alt)）：

```
void f1(const std::string& s);      // Pass by reference-to-const
void f2(const std::string* sptr);   // Pass by pointer-to-const
void f3(std::string s);             // Pass by value 
```

按值传参时，函数内部会获得一个参数的完整副本，内部对副本的改动不会影响到调用者。当参数是一个比较大的对象时，这里的复制会花费很多资源。（[Difference between const reference and normal parameter](https://stackoverflow.com/questions/2627166/difference-between-const-reference-and-normal-parameter)）

大部分情况下，当不改变参数值时，采用`Pass by reference-to-const`的方法，这样的话语义比较清晰（引用只是一个语法糖，使用引用可读性更高）。不过以下两种例外情况，要采用 `Pass by pointer-to-const` 的方法（[What are the differences between a pointer variable and a reference variable in C++?](https://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable-in/57492#57492)）：

* 有时候可能会传递 nullptr；
* 函数内部需要保存指向输入参数的指针或者引用。（指针有自己的内存地址，引用没有）

