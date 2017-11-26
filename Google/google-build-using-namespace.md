```
#include <iostream>
// Do not use namespace directive
using namespace std;

int main(){
    cout << "build using namespace" << endl;
}

```

C++引入命名空间(namespace)概念的主要目的是避免命名冲突。但是当我们想要引用命名空间成员时，就会体会到相比直接使用变量或函数的不便之处，特别是当命名空间名字很长时，更是如此。所以针对这类问题，有三种解决方案： 

1. using declarations（using声明）：这种方式每次只引入一个命名空间成员，形式是 `using namespace_name::member_name`，它服从一般的作用域法则，从using declarations开始一直到当前作用域结束，都是可见的。
2. namespace aliases （命名空间别名）： 对于一个名字过长的命名空间也可以指定别名， `namespace short_name = origin_name`，一个命名空间可以有多个别名，而且别名和原始名之间可以交换使用。
3. using directives（using指令）：这种方式使得来自某个特定命名空间的所有命名在当前作用域都是可见的，形式是 `using namespace namespace_name;`。 

虽然 using directives 方式对于编程者而言，似乎使用起来更加方便（只要在全局变量中使用using namespace namespace_name，之后该命名空间下的所有命名都可以使用简写形式），但是最好不要那么做。因为当我们引用多个库时，采用using directives的方式，又会重新引起命名冲突问题，那么命名空间也就失去了它最初的作用。

## Ref  

[Why “using namespace X;” is not allowed inside class/struct level?](https://stackoverflow.com/questions/6326805/why-using-namespace-x-is-not-allowed-inside-class-struct-level)  

