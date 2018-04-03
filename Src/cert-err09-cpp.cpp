#include <iostream>

void throw_by_pointer() {
    int num = 2;
    int *num_ptr = &num;
    std::cout << num_ptr << std::endl;
    std::cout << *num_ptr << std::endl;
    throw num_ptr;              // Throw a pointer is dangerous.
}

class ClassA {
public:
    virtual void PrintMe() const {
        std::cout << "ClassA" << std::endl;
    }
};

class ClassB : public ClassA {
public:
    void PrintMe() const override {
        std::cout << "ClassB" << std::endl;
    }
};

void throw_by_value() {
    auto *obj = new ClassB;
    throw *obj;
}

int main() {
    try {
        throw_by_pointer();
    } catch(const int *p) {
        std::cout << p << std::endl;
        std::cout << *p << std::endl;
    }

    try {
        throw_by_value();
    }
    catch(const ClassA a) {
        //  Catching by value results in slicing at the catch site
        a.PrintMe();
    }

    try {
        throw_by_value();
    }
    catch(const ClassA &a) {
        // Catching by reference preserves the polymorphism of the exception object.
        a.PrintMe();
    }
    return 0;
}

/*
0x7ffee9eed54c
2
0x7ffee9eed54c
167772160  (not predictable)
ClassA
ClassB
 */
