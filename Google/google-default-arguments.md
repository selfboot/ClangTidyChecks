```c++
#include <iostream>

class Animal {
public:
    // 1.
    virtual void show(int data = 10) {
        std::cout << "Animal--data=" << data << std::endl;
    }
};

class Person : public Animal {
public:
    // 2.
    void show(int data = 20) override {
        std::cout << "Person--data=" << data << std::endl;
    }
};

int main() {
    Person *person_ptr = new Person();
    Animal *animal_ptr = person_ptr;
    person_ptr->show();
    animal_ptr->show();
    delete person_ptr;
    return 0;
}
```

上面show函数为虚函数，并且有默认参数。main函数中调用时，参数是静态编译期决定，运行期用虚函数表机制决定调用基类还是派生类的show函数（[Can virtual functions have default parameters?](https://stackoverflow.com/questions/3533589/can-virtual-functions-have-default-parameters)）。

所以上面函数的输出为

```shell
Person--data=20
Person--data=10
```

[Google规范](https://google.github.io/styleguide/cppguide.html#Default_Arguments)认为使用带有默认参数值的虚函数是不当的行为，因为虚函数的本意是实现多态，而默认参数静态绑定打破了这一语义。Clang-tidy会对上面1，2处代码中默认参数的使用进行报警。CERT规范（[OOP04-CPP. Prefer not to give virtual functions default argument initializers](https://www.securecoding.cert.org/confluence/display/cplusplus/OOP04-CPP.+Prefer+not+to+give+virtual+functions+default+argument+initializers)）也建议不要在虚函数中使用默认参数值。

## default argument

C++11 关于虚函数默认参数的规定如下（8.3.6.10）：

> A virtual function call uses the default arguments in the declaration of the virtual function determined by the static type of the pointer or reference denoting the object. An overriding function in a derived class does not acquire default arguments from the function it overrides. 

除了指出根据调用对象的静态类型来决定默认参数的值，还指出派生类的虚函数可以不用指定默认值（[Virtual functions default parameters](https://stackoverflow.com/questions/18105523/virtual-functions-default-parameters)）。如下例子：

```
class Animal {
public:
    virtual void show(int data = 10) {
        std::cout << "Animal--data=" << data << std::endl;
    }
};

class Man : public Animal{
public:
    void show(int data) override {
        std::cout << "Man--data=" << data << std::endl;
    }
};

int main() {
    Man *man_ptr = new Man();
    Animal* animal_ptr2 = man_ptr;
    // man_ptr->show();         //error: wrong number of arguments for B::f()
    animal_ptr2->show();

    delete man_ptr;
    return 0;
}
```

派生类Man的虚函数show参数data并没有默认值，这也是可以的，不过这样调用时就必须提供参数值，不能使用默认参数值。

此外关于默认参数，C++标准 8.3.6.9 规定如下：

> Default arguments are evaluated each time the function is called. The order of evaluation of function arguments is unspecified. Consequently, parameters of a function shall not be used in default argument expressions, even if they are not evaluated.

所以如下的代码是错误的（[C++ default argument set as a previous argument](https://stackoverflow.com/questions/1880866/c-c-default-argument-set-as-a-previous-argument)）：

```c++
int f(int a, int b = a); // error: parameter a used as default argument
```

