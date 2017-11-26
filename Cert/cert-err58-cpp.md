```c++
#include <iostream>

struct Demo {
    explicit Demo(int i) noexcept(false): data(i) {
        if(0 == i) {
            throw (i);
        }
    };
    int data;
};

// The constructor for Demo may throw an exception that is not caught
// when d is constructed during program startup.
static Demo d(0);     // terminating with uncaught exception of type int
//Demo e(0);          // terminating with uncaught exception of type int
//static Demo d(1);   // OK.

int func(int i) noexcept(false) {
    if(0 == i) {
        throw (i);
    }
    return i;
};

//  An exception may be thrown by the initializer for the global variable num
int num = func(0);        // terminating with uncaught exception of type int
//int i = func(i);      // OK.

int main() {
    std::cout << "In main() function: d.data is " << d.data << std::endl;
    std::cout << "In main() function: num is " << num << std::endl;
    return 0;
}
```

上面代码中全局对象 d 构造过程抛出异常，全局变量num初始化过程抛出异常都会导致程序在进入 main 函数之前崩溃。因为这里的异常无法被捕获，所以会调用terminate函数终止程序的执行。C++中不是所有的异常都能够被捕获，C++标准 [except.handle] 中明确说明：

> Exceptions thrown in destructors of objects with static storage duration or in constructors of namespace scope objects with static storage duration are not caught by a function-try-block on main() . Exceptions thrown in destructors of objects with thread storage duration or in constructors of namespace-scope objects with thread storage duration are not caught by a function-try-block on the initial function of the thread.

对于上面的两种情况，代码做如下更改即可：

```c++
#include <iostream>

struct Demo {
    explicit Demo(int i) noexcept(false): data(i) {
        if(0 == i) {
            throw (i);
        }
    };
    int data;
};

/*
 * Make d into a local variable with static storage duration,
 * allowing any exceptions thrown during object construction to be caught
 * because the constructor for Demo will be executed the first time
 * the function CreateDemo() is called rather than at program startup.
 */
Demo &CreateDemo(int i) {
    try {
        static Demo d(i);
        return d;
    } catch (...) {
        // Handle error, perhaps by logging it and gracefully terminating the application.
        std::cout << "Create Demo failed because of an exception. " << std::endl;
        std::terminate();
        exit(-1);
    }
}
Demo d = CreateDemo(0);

int func(int i) noexcept(false) {
    if(0 == i) {
        throw (i);
    }
    return i;
};

/*
 * Wrap the call to f() with a helper function that catches all exceptions and terminates the program.
 */
int func_helper(int i) noexcept {
    try {
        return func(i);
    } catch (...) {
        std::cout << "Call func failed because of an exception. " << std::endl;
        exit(-1);
    }
}

int num = func_helper(0);

int main() {
    std::cout << "In main() function." << std::endl;
    std::cout << "d.data is " << d.data << std::endl;
    std::cout << "num is " << num << std::endl;
    return 0;
}
```

## Storage Duration

注意C++标准 [except.handle] 中提到 static storage duration 和 thread storage duration 对象，是什么意思呢？



哪些变量拥有


1. 非thread、非dynamic所有的非局部变量
2. 用static声明的局部变量和类成员变量
thread
thread_local 声明的所有变量


详细内容可以参考 C++标准 [Storage duration] 3.7


## noexcept(C++11)


[When should I really use noexcept?](https://stackoverflow.com/questions/10787766/when-should-i-really-use-noexcept)  

