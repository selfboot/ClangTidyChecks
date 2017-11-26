
```
#include <iostream>
#include <vector>

int main() {
    std::string s = "abc";

    // 1 ok
    std::pair<std::string, int> a = std::make_pair(s, 7);

    // 2 error.  no matching function for call to 'make_pair'
    // std::pair<std::string, int> b = std::make_pair<std::string, int>(s, 7);

    // 3 ok
    std::pair<std::string, int> c = std::pair<std::string, int>(s, 7);

    // 4 Warning.
    std::pair<int, int> d = std::make_pair<int, int>(2, 7);
    return 0;
}
```

g++ 4.4上面代码可以正常编译运行，G++ 4.7如果带有 -std=c++11，上述代码2编译错误。如果用clang-tidy检测，4有警告信息。总结：C++ 11 中，make_pair 不能显式指定**模板参数**。

C++ 11中std::make_pair函数有两个参数T&&和U&&，这里T和U都是模版类型参数，具体函数声明如下：

```
template <typename T， typename U>
[return type] make_pair(T&& argT， U&& argU);
```

当我们调用make_pair时显式指明参数类型时，C++不会使用**模板推导**，而是直接用指定的参数替换模板类型。上面2对应调用时，make_pair实际的函数定义如下：

```
[return type] make_pair(std::string&& argT， int&& argU);
```

可以看到两个参数的类型均为**右值引用**，因此只能绑定右值，第一个参数s明显不是右值引用，因此报错。

## template argument deduction

在开始的代码中，如果直接使用以下的方式调用make_pair则没有任何问题，其中s 是std::string类型的左值，7是int类型的右值。 

```
make_pair(s， 7)
```

因为没有显式指定参数类型，因此C++编译器使用模板类型推导（template argument deduction）来找出类型。参数s为左值，编译器首先推导T为`std::string&`（string类型的左值参数），这样参数 T&& 展开后实际类型为 `std::string& &&`，因为不存在对于引用的引用，这里多个引用可以**折叠(Reference Collapsing Rules)**起来，所以参数 T&& 类型最后的推导结果是 std::string&，和 s 是匹配的。同理，参数7是整型int，编译器首先推导U为int，这样U&&就会变成int &&，和右值7也是匹配的。

总结来说，模板右值引用（**universal reference**）比较特殊，因为`reference collapsing`语言特性，对于 T&& 这种模板类型的右值引用（T是模板参数），可以绑定任何类型的T。不管T是lvalue， rvalue， const-qualified， volatile-qualified， or unqualified，T都可以推导为T&&。

> If a template argument can be deduced from the function arguments， let it be deduced. Don't explicitly provide the argument unless you absolutely must.
> 
> Let the compiler do the hard work， and 99.9% of the time it'll be exactly what you wanted anyway. When it isn't what you wanted， you'll usually get a compilation error which is easy to identify and fix.

## lvalue， rvalue reference

C++定义了与C不相同的左值和右值的判断方法，不过说起来非常简单：凡是真正的存在内存当中，而不是寄存器当中的值就是左值，其余的都是右值。其实更通俗一点的说法就是：凡是取地址（&）操作可以成功的都是左值，其余都是右值。

```c++
// lvalues:
int i = 42;
i = 43; // ok， i is an lvalue 
int* p = &i; // ok， i is an lvalue 
int& foo();
foo() = 42; // ok， foo() is an lvalue
int* p1 = &foo(); // ok， foo() is an lvalue
// rvalues: 
int foobar(); 
int j = 0;
j = foobar(); // ok， foobar() is an rvalue
int k = j + 2; // ok， j+2 is an rvalue
int* p2 = &foobar(); // error， cannot take the address of an rvalue 
j = 42; // ok， 42 is an rvalue
```

**对于左值的引用就是左值引用，而对于右值的引用就是右值引用**。C++11中，用int&表示对于int类型的左值引用，使用int&&来表示对于int类型的右值引用。下面是一组重载的函数：

```c++
void foo(const int & i) { cout << "const int & " << i << endl; }
void foo(int & i) { cout << "int & " << i << endl; }
void foo(int && i) { cout << "int && " << i << endl; }
void foo(const int && i) { cout << "const int && " << i << endl; } 
```

大多时候，我们使用第一种形式，它（const int &）既可以绑定左值，也可以绑定右值。所以在没有后面三个重载函数的情况下，我们调用以下语句：

```c++
int i = 2;
foo(i);
foo(2);
```

他们的输出都是`const int & 2`。而如果在只有第二个函数而没有其他函数的时候，第三条语句是违法的。在只有第三个函数没有其它函数的时候，第二条语句是违法的。总结来说：**const reference可以绑定所有的值，而其他类型的引用只能绑定自己类型的值**。在四个函数都存在的情况下，每一个函数都会绑定与自己最接近的那个值。也就是说，在四个函数都存在的情况下，当我们运行下面的这段代码：

```c++
int i=2;
const int j=2;
foo(i);
foo(j);
foo(2);
foo([]()->const int && {return 2;}());
```

输出的结果就将变成：

```bash
int & 2
const int & 2
int && 2
const int && 2
```

第四条语句编译的时候会有Warning，提示“Returning reference to local temporary object”。常量右值引用其实不太能想出什么应用场景，不一定有什么实际意义，所以这个就不要纠结了。

# 通用引用（universal reference）

是Scott Meyers在C++ and Beyond 2012演讲中自创的一个词，用来特指一种引用的类型。构成通用引用有两个条件：

必须满足T&&这种形式
类型T必须是通过推断得到的

## Ref

[C++11 make_pair with specified template parameters doesn't compile](https://stackoverflow.com/questions/9641960/c11-make-pair-with-specified-template-parameters-doesnt-compile)   
[移动语义（move semantic）和完美转发（perfect forward）](https://codinfox.github.io/dev/2014/06/03/move-semantic-perfect-forward/)  
[Template argument deduction](http://en.cppreference.com/w/cpp/language/template_argument_deduction)  
[C++11 模板参数推导（Template Argument Deduction）是如何工作的？](https://www.zhihu.com/question/28024010)  
[What does “cv-unqualified” mean in C++?](https://stackoverflow.com/questions/15413037/what-does-cv-unqualified-mean-in-c)  

