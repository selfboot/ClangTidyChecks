```c++
#include <iostream>
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

void print_number(const int* myInt) {
    assert(myInt!=nullptr);                         // Assert in runtime
    std::cout << *myInt << std::endl;
}

void check_env(){
    int i;
    assert(sizeof(void *) >= sizeof i);             // Assert in compile time
    // static_assert(sizeof(void *) >= sizeof i);   // Better using static_assert
}

int main ()
{
    int a=10;
    int *b = &a;
    int *c = nullptr;

    print_number(b);
    print_number(c);

    check_env();
    return 0;
}
```



## assert vs static_assert

C++ 中 assert 定义在头文件 `<cassert>` 中，原型定义：

```c++
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif
```

assert的作用是计算表达式 condition ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。我们通常在调试阶段使用assert，不过频繁的调用 assert 会极大地影响程序的性能，增加额外的开销。因此在调试结束后，需要在#include <assert.h>语句之前插入 `#define NDEBUG` 来禁用assert调用。



[What does static_assert do, and what would you use it for?](https://stackoverflow.com/questions/1647895/what-does-static-assert-do-and-what-would-you-use-it-for)  

