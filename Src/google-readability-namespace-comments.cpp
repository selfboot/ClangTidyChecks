#include <iostream>

// 1. Namespace foo not terminated with a closing comment.
namespace foo{
    int num = 1;
}

namespace bar{
    int num = 2;
} // namespace bar

int main(){
    std::cout << foo::num << std::endl << bar::num << std::endl;
}
