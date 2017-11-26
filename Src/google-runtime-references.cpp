#include <iostream>

// 1. parameter as reference
void foo(std::string &in, std::string *out){
    *out = in;
    std::string *demo = out;
    std::cout << demo << std::endl;

}

int main(){
    std::string in = "src";
    std::cout << &in << std::endl;

    std::string out = "dest";
    foo(in, &out);

    std::cout << out << std::endl;
    return 0;
}
