完整代码参考 [cert-err34-c.cpp](../Src/cert-err34-c.cpp)

```c++
#include <iostream>

int Str2Int(const char *buff) {
    int num = 0;
    if (buff) {
        // http://www.cplusplus.com/reference/cstdlib/atoi/
        num = atoi(buff);
    }
    return num;
}

int main(){
    std::cout << Str2Int("11a") << std::endl;
    std::cout << Str2Int("  -11a") << std::endl;
    std::cout << Str2Int("+011a") << std::endl;

    // Following all return 0.
    std::cout << Str2Int("  ") << std::endl;
    std::cout << Str2Int("abcd") << std::endl;
    std::cout << Str2Int("0") << std::endl;

    // It causes undefined behavior when out of the range of representable values by an int.
    std::cout << Str2Int("37444444777777777777") << std::endl;
}
```

C语言中可以用 `atoi()` 将const char* 字符串转换为 int，不过该方法并不安全，主要体现在两个方面：

1. 如果字符串不能转换为int，该方法返回0，而当字符串是 "0" 或者 "  0" 时，也是返回0，无法区分这两种情况；
2. 字符串转换失败时不会设置 `errno`；
2. 如果字符串表示的数字超出范围，将**导致未定义行为**。

类似的转换函数还有 `atol()`, `atoll()`, `atof()`, `scanf()` 等等。

# strtol

我们应该避免使用 atoi 等不进行异常检测的一系列函数，采用 [strtol](http://www.cplusplus.com/reference/cstdlib/strtol/) 等函数来进行字符串到整型的安全转换。

```c++
bool Str2IntSafe(const char *buff, int &ret) {
    char *end;
    errno = 0;

    const int64_t num = strtol(buff, &end, 10);

    if (end == buff) {
        std::cout << buff << ": not a decimal number\n";
    } else if ('\0' != *end) {
        std::cout << buff << ": extra characters at end of input: " << end << std::endl;
    } else if ((LONG_MIN == num || LONG_MAX == num) && ERANGE == errno) {
        std::cout << buff << ": out of range of type long\n";
    } else if (num > INT_MAX) {
        std::cout << num << ": greater than INT_MAX\n";
    } else if (num < INT_MIN) {
        std::cout << num << ": less than INT_MIN\n";
    } else {
        ret = static_cast<int>(num);
        return true;
    }
    return false;      // 字符串不是 int 类型
}
```

详细示例可以参考 [cert-err34-c_compliant.cpp](../Src/cert-err34-c_compliant.cpp)。

# stoi 

C++11提供了 [std:stoi](http://en.cppreference.com/w/cpp/string/basic_string/stol), std::stol, std::stoll函数，可以安全地将string类型字符串转换为整型。当转换失败时，会抛出异常。 

```c++
#include <iostream>
#include <vector>

bool Str2Long(const std::string& str, int64_t *num){
    try {
        *num = std::stoi(str);
    }
    catch(const std::invalid_argument &e){
        std::cout << "std::stoi(\"" << str << "\") throw std::invalid_argument " << '\n';
        return false;
    }
    catch(const std::out_of_range &e){
        std::cout << "std::stoi(\"" << str << "\") throw std::out_of_range" << '\n';
        return false;
    }
    return true;
}

int main()
{
    std::vector<std::string> strs= {"+11 with words", "3.1415",
                                    "not valid num", "12345678901011111111"};
    int64_t tmp;
    for(auto &str : strs){
        if(Str2Long(str, &tmp)){
            std::cout << "std::stoi(\"" << str << "\") is " << tmp << '\n';
        }
    }
    return 0;
}
```

更多内容可以参考 [SO:C++ - error while using atoi](https://stackoverflow.com/questions/13470822/c-error-while-using-atoi).

