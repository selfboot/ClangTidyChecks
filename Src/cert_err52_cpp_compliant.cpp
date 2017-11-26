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
