```c++
// Inputs/global-names-in-headers.h
#ifndef CLANG_TIDY_TESTS_GLOBAL_NAMES_IN_HEADERS_H
#define CLANG_TIDY_TESTS_GLOBAL_NAMES_IN_HEADERS_H

#include <iostream>

using namespace std;    // 1.
using std::cout;        // 2.

namespace foo{
    using std::endl;    // 3.
} // namespace foo

#endif //CLANG_TIDY_TESTS_GLOBAL_NAMES_IN_HEADERS_H

// google-global-names-in-headers.cpp
#include "Inputs/global-names-in-headers.h"

int main(){
    cout << 1 << foo::endl;
}
```

上面代码在头文件中使用了 using 声明和 using 指令，对于全局变量命名空间来说（1，2），这样做很可能会污染命名空间，因此应该避免（[“using namespace” in c++ headers](https://stackoverflow.com/questions/5849457/using-namespace-in-c-headers)）。对于内部命名空间来说，这样做是没有问题的。

C++ Coding Standards: 101 Rules, Guidelines, and Best Practices"

> Don’t write namespace usings in a header file or before an #include.


