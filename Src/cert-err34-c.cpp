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
