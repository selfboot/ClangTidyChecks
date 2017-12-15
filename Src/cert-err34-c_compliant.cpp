#include <cerrno>
#include <climits>
#include <cstdlib>
#include <iostream>

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

int main(){
    int num = 0;
    if(Str2IntSafe("11a", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("  -11a", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("+011", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("  ", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("abcd", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("0", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("37444444777777", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
    if(Str2IntSafe("374444447777777777770000000000000000000000000", num)){
        std::cout << "Get int safe: " << num << std::endl;
    }
}
