#include <iostream>

struct Demo {
    explicit Demo(int i) noexcept(false): data(i) {
        if(0 == i) {
            throw (i);
        }
    };
    int data;
};

// The constructor for Demo may throw an exception that is not caught
// when d is constructed during program startup.
static Demo d(0);     // terminating with uncaught exception of type int
//Demo e(0);          // terminating with uncaught exception of type int
//static Demo d(1);   // OK.

int func(int i) noexcept(false) {
    if(0 == i) {
        throw (i);
    }
    return i;
};

//  An exception may be thrown by the initializer for the global variable num
int num = func(0);        // terminating with uncaught exception of type int
//int i = func(i);      // OK.

int main() {
    std::cout << "In main() function: d.data is " << d.data << std::endl;
    std::cout << "In main() function: num is " << num << std::endl;
    return 0;
}
