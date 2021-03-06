完整的代码参考 [../Src/cert-dcl58-cpp](cert-dcl58-cpp.cpp)

```c++
#include <cmath>
#include <iostream>
#include <vector>

struct point {
    float x;
    float y;
};

namespace std {
    float distance(const point &p1, const point &p2) {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                    (p1.y - p2.y) * (p1.y - p2.y));
    }
} // namespace std

int main() {
    point p1 = {0, 0};
    point p2 = {1, 1};
    std::cout << std::distance(p1, p2) << std::endl;            // Ok, seems right.

    std::vector<point> vec;
    vec.push_back(p1);
    vec.push_back(p2);
    for(auto i = vec.begin(); i != vec.end(); i++) {
        for(auto j = vec.begin(); j != vec.end(); j++) {
            std::cout << std::distance(i, j) << std::endl;       // Call distance in standard library.
            // std::cout << std::distance(*i, *j) << std::endl;  // This is the right way to call your func.
        }
    }
}
```

> **C++ Standard**: **Do not** add declarations or definitions to the standard namespaces std or posix, or to a namespace contained therein, except for a template specialization that depends on a user-defined type that meets the standard library requirements for the original template.

## Namespaces std




参考 [Why is “using namespace std” considered bad practice?](https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice)  

