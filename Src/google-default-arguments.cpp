#include <iostream>

class Animal {
public:
    virtual void show(int data = 10) {
        std::cout << "Animal--data=" << data << std::endl;
    }
};

class Person : public Animal {
public:
    void show(int data = 20) override {
        std::cout << "Person--data=" << data << std::endl;
    }
};

class Man : public Animal{
public:
    void show(int data) override {
        std::cout << "Man--data=" << data << std::endl;
    }
};

int main() {
    Person *person_ptr = new Person();
    Animal *animal_ptr = person_ptr;
    person_ptr->show();
    animal_ptr->show();

    Man *man_ptr = new Man();
    Animal* animal_ptr2 = man_ptr;
    // man_ptr->show();         //error: wrong number of arguments for B::f()
    animal_ptr2->show();

    delete man_ptr;
    delete person_ptr;
    return 0;
}

/*
Person--data=20
Person--data=10
Man--data=10
 */
