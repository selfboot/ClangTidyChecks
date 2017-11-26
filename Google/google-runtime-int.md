```c++
#include <iostream>

int main(){

    int one = 1;                // 1. ok
    short two = 1;              // 2.
    unsigned short three = 2;   // 3.

    long four = 10000;          // 4.
    long long five = 100000;    // 5.
    unsigned long long six = 10;// 6.

    int64_t foo = 200000;
    return 0;
}
```

上面代码中定义了 short，long，long long等一系列变量，看着没问题。不过Google（[Integer Types](https://google.github.io/styleguide/cppguide.html#Integer_Types)）认为这样不够严谨，应该使用 int16_t，int64_t，指定具体数字用到的二进制位数。

```c++
int8_t ch = 1;
int16_t foo_16 = 1;
int64_t foo = 200000;
```

## Integer Types

整型相关的数据类型在不同位数机器的平台下长度不同。C99标准并不规定具体数据类型的长度大小，一般认为short是16位，int是32位，long是32位，long long是64位。有时候需要精确指定整数的精度，这时候可以使用固定宽度整型。C99头文件stdint.h，以及C++11头文件cstdint中定义了一系列固定宽度的整型，如下所示：

* int8_t
* int16_t
* int32_t
* int64_t
  
为了保证平台的通用性，程序中尽量不要使用long，short等类型，而是使用固定长度的类型。

## Ref

[Google: Integer Types](https://google.github.io/styleguide/cppguide.html#Integer_Types)   
[What does the C++ standard state the size of int, long type to be?](https://stackoverflow.com/questions/589575/what-does-the-c-standard-state-the-size-of-int-long-type-to-be)  
[Fixed width integer types](http://en.cppreference.com/w/cpp/types/integer) 

