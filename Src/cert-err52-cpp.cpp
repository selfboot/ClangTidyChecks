#include <csetjmp>
#include <iostream>

static jmp_buf env;

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
        longjmp(env, 1);           // throw
    }
    std::cout << "f(): Instances(After throw): " << Counter::instances << std::endl;
}

int main() {
    std::cout << "Before setjmp(): Instances: " << Counter::instances << std::endl;
    if(setjmp(env) == 0) {              // try
        f(0);
    } else {                            // catch
        std::cout << "From longjmp(): Instances: " << Counter::instances << std::endl;
    }

    std::cout << "After longjmp(): Instances: " << Counter::instances << std::endl;
}
