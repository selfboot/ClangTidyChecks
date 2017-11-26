# cert-err52-cpp

```
#include <csetjmp>
#include <iostream>

static jmp_buf env;

struct Counter {
    static int instances;
    Counter() { ++instances; }
    ~Counter() { --instances; }
};

int Counter::instances = 0;

void f(int i) {
    Counter c;
    std::cout << "f(): Instances: " << Counter::instances << std::endl;
    if(i==0){
        longjmp(env, 1);        // throw
    }
    std::cout << "f(): Instances(After throw): " << Counter::instances << std::endl;
}

int main() {
    std::cout << "Before setjmp(): Instances: " << Counter::instances << std::endl;
    if(setjmp(env) == 0) {      // try
        f(0);
    } else {                    // catch
        std::cout << "From longjmp(): Instances: " << Counter::instances << std::endl;
    }

    std::cout << "After longjmp(): Instances: " << Counter::instances << std::endl;
}
```

上面代码利用C中的setjmp和longjmp来实现异常处理，导致在f()中Counter的临时对象c的析构函数没有被调用。[C++标准 Other runtime support. 18.10.4](http://www.lcdf.org/c++/) 中明确说明：

> The function signature longjmp(jmp_buf jbuf, int val) has more restricted behavior in this International Standard. A setjmp/longjmp call pair has undefined behavior if replacing the setjmp and longjmp by catch and throw would invoke any non-trivial destructors for any automatic objects.

因此 C++ 中**不要使用 setjmp 和 longjmp**，用标准的异常处理机制来代替（[ERR52-CPP. Do not use setjmp() or longjmp()](https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=88046492)）。上面的程序做以下更改即可：

```
#include <iostream>

struct Counter {
    static int instances;

    Counter() { ++instances; }

    ~Counter() { --instances; }
};

int Counter::instances = 0;

void f(int i) {
    Counter c;
    std::cout << "f(): Instances: " << Counter::instances << std::endl;
    if(i==0){
        throw "Exception";
    }
    std::cout << "f(): Instances(After throw): " << Counter::instances << std::endl;
}

int main() {
    std::cout << "Before throw: Instances: " << Counter::instances << std::endl;
    try {
        f(0);
    } catch(const char *E) {
        std::cout << "From catch: Instances: " << Counter::instances << std::endl;
    }
    std::cout << "After catch: Instances: " << Counter::instances << std::endl;
}
```

关于C中 setjmp和longjmp 的详细内容，可以参考 [Exceptions in C with Longjmp and Setjmp](http://www.di.unipi.it/~nids/docs/longjump_try_trow_catch.html) 一文。此外，C中setjmp和longjmp 的使用可以参考 [Practical usage of setjmp and longjmp in C](https://stackoverflow.com/questions/14685406/practical-usage-of-setjmp-and-longjmp-in-c)。

