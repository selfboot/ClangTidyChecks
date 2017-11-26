#include <iostream>
#include <vector>

int main() {
    std::string s = "abc";

    // 1 ok
    std::pair<std::string, int> a = std::make_pair(s, 7);

    // 2 error.  no matching function for call to 'make_pair'
    // std::pair<std::string, int> b = std::make_pair<std::string, int>(s, 7);

    // 3 ok
    std::pair<std::string, int> c = std::pair<std::string, int>(s, 7);

    // 4 Warning.
    std::pair<int, int> d = std::make_pair<int, int>(2, 7);
    return 0;
}
