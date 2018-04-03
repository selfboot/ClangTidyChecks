#include <exception>
#include <iostream>
#include <type_traits>

class BadParam : public std::exception {
    std::string data_;
public:
    explicit BadParam(const char *msg) : data_(msg) {
        std::cout << "Constructor func" << std::endl;
    }

    const char *what() const noexcept override {
        return data_.c_str();
    }

    // Copy constructor for the exception object type throws exception here.
    BadParam (const BadParam& another) {
        std::cout << "Call copy constructor" << std::endl;
        this->data_ = another.data_;
        throw std::bad_alloc();
    };
};

void ThrowFunc() {
    // BadParam tmp("Should Call copy constructor for the exception object here.");
    // throw tmp;
    throw BadParam("Not call copy constructor for the exception object here");
}

void CalledFunc() {
    try {
        ThrowFunc();
    } catch(BadParam &s) {
        std::cout << s.what() << std::endl;
        std::cout << "Handle error here." << std::endl;
    }
}

int main(){
    // Test whether the copy constructor and copy assignment of exception is noexcept,
    static_assert(std::is_nothrow_copy_constructible<BadParam>::value,
                  "BadParam is nothrow copy constructible");
    // static_assert(std::is_nothrow_copy_assignable<BadParam>::value, "");
    CalledFunc();
}

