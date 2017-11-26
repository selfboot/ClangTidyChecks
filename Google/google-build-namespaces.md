```c++
// Inputs/google-namespaces.h
#ifndef CLANG_TIDY_TESTS_GOOGLE_NAMESPACES_H_H
#define CLANG_TIDY_TESTS_GOOGLE_NAMESPACES_H_H

namespace {
    int v;
}

#endif //CLANG_TIDY_TESTS_GOOGLE_NAMESPACES_H_H
 
// google-namespace.cpp
#include "Inputs/google-namespaces.h"
#include <iostream>
 
void f() {
  std::cout << "f(): " << v << std::endl;
  v = 42;
}
 
// google-build-namespaces.cpp
#include "Inputs/google-namespaces.h"
#include <iostream>
 
void g() {
  std::cout << "g(): " << v << std::endl;
  v = 100;
}
 
int main() {
  extern void f();
  f(); // Prints v， sets it to 42
  g(); // Prints v， sets it to 100
  f();
  g();
}
/*
f(): 0
g(): 0
f(): 42
g(): 100
*/
```

优秀的C++项目代码中，通常使用**头文件**来存放不同编译单元共享的代码块。如果在头文件中定义了 unnamed namespace，将会导致意料之外的结果。因为每个编译单元unnamed namespace中声明或者定义的变量、函数只有自己可以访问，因此包含了该头文件的每个编译单元都会定义属于自己的实例，从而导致数据完整性问题，甚至是性能问题。

> Unnamed namespaces are used to define a namespace that is unique to the translation unit， where the names contained within have internal linkage by default.

## unnamed namespaces

没有指定名字的namespace被称为一个匿名namespace；在一个编译单元中可以出现多个匿名namespace，并且相同层次的匿名namespace实际上被合成为同一个。出现在不同编译单元的匿名namespace中的相同标识符相互独立不会发生冲突，因此我们可以把那些只希望在同一个编译单元范围可见的全局标识符放入一个匿名namespace中。

C++中，一个全局标识符被static修饰后它的linkage变为internal linkage，这就是为什么不同编译单元中的相同标识符不会发生冲突的原因。匿名namespace可以用来实现和static相同的效果，不同标准实现略微不同。

* 对于C++03来说，不会改变在它内部定义的标识符的linkage属性，它用来避免名字冲突所采用的手段同C++用来实现重载的手段一摸一样，就是使用`name mangling`：每个编译单元中的匿名namespace实际上会拥有一个独一无二的名字，因此在不同编译单元的匿名namespace中相同的标识符实际上属于不同的namespace，自然不会发生冲突。
* 对于C++11来说，匿名命名空间会将内部定义的标识符的linkage改为internal，和static做法一样，不过还支持自定义的类，因此更加强大（[Superiority of unnamed namespace over static?](https://stackoverflow.com/questions/4422507/superiority-of-unnamed-namespace-over-static)）。

## external， internal linkage

[Internal and External Linkage in C++](http://www.goldsborough.me/c/c++/linker/2016/03/30/19-34-25-internal_and_external_linkage_in_c++/)  

当一个实现文件(.cpp ...)编译时，预处理器(CPP)首先递归的包含头文件，形成一个保含有所有必要信息的单个源文件。这个源文件称为 编译单元。这个编译单元会被编译成为一个与cpp 文件名同名的目标文件(.o或是.obj) 。链接程序把不同编译单元中产生的符号联系起来，构成一个可执行程序。

Internal linkage（内部链接）：如果一个符号（symbol）对于它的编译单元来说是局部的，并且在链接的时候不可能与其它编译单元中的同样的名称相冲突，则这个名称具有内部链接。即具有内部链接的名称不会引入到目标文件中。

External linkage（外部链接）：在一个多源文件项目中，如果一个符号（symbol）在链接时可以和其他编译单元交互，那么这个名称就具有外部链接。即具有外部链接的符号会引入到目标文件中，由有链接器进行处理。那么，这种符号在整个程序中必须是惟一的，不然会有冲突。

可以使用extern和static关键字显式控制符号的链接属性。如果未指定链接，那么对于非const符号默认是extern链接，对于const符号默认是static（internal）链接。

1. 非 const 全局变量默认情况下具有外部链接
2. 默认情况下，const全局变量具有内部链接
3. 默认情况下，函数有外部链接

如下代码所示：

```c++
// in namespace or global scope
int i; // extern by default
const int ci; // static by default
extern const int eci=0; // explicitly extern
static int si; // explicitly static

// the same goes for functions (but there are no const functions)
int foo(); // extern by default
static int bar(); // explicitly static 
```

## Ref

[Namespaces](https://google.github.io/styleguide/cppguide.html#Namespaces)  
[Unnamed/anonymous namespaces vs. static functions](https://stackoverflow.com/questions/154469/unnamed-anonymous-namespaces-vs-static-functions)   
[Are objects in anonymous namespace implicitly static?](https://stackoverflow.com/questions/13396748/are-objects-in-anonymous-namespace-implicitly-static)    
[What is external linkage and internal linkage?](https://stackoverflow.com/questions/1358400/what-is-external-linkage-and-internal-linkage)  
[DCL59-CPP. Do not define an unnamed namespace in a header file](https://www.securecoding.cert.org/confluence/display/cplusplus/DCL59-CPP.+Do+not+define+an+unnamed+namespace+in+a+header+file)  
[Superiority of unnamed namespace over static?](https://stackoverflow.com/questions/4422507/superiority-of-unnamed-namespace-over-static)  
[Why are unnamed namespaces used and what are their benefits?](https://stackoverflow.com/questions/357404/why-are-unnamed-namespaces-used-and-what-are-their-benefits)  

