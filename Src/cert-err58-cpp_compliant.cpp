#include <iostream>

struct Demo {
    explicit Demo(int i) noexcept(false): data(i) {
        if(0 == i) {
            throw (i);
        }
    };
    int data;
};

/*
 * Make d into a local variable with static storage duration,
 * allowing any exceptions thrown during object construction to be caught
 * because the constructor for Demo will be executed the first time
 * the function CreateDemo() is called rather than at program startup.
 */
Demo &CreateDemo(int i) {
    try {
        static Demo d(i);
        return d;
    } catch (...) {
        // Handle error, perhaps by logging it and gracefully terminating the application.
        std::cout << "Create Demo failed because of an exception. " << std::endl;
        std::terminate();
        exit(-1);
    }
}
Demo d = CreateDemo(0);

int func(int i) noexcept(false) {
    if(0 == i) {
        throw (i);
    }
    return i;
};

/*
 * Wrap the call to f() with a helper function that catches all exceptions and terminates the program.
 */
int func_helper(int i) noexcept {
    try {
        return func(i);
    } catch (...) {
        std::cout << "Call func failed because of an exception. " << std::endl;
        exit(-1);
    }
}

int num = func_helper(0);

int main() {
    std::cout << "In main() function." << std::endl;
    std::cout << "d.data is " << d.data << std::endl;
    std::cout << "num is " << num << std::endl;
    return 0;
}

