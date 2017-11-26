#include <iostream>

class Foo {
    int data=0;
 public:
    Foo operator++(int) {
        Foo R(*this);
        data++;
        return R;
    }
    int getData() const { return data; }
};

int main() {
    Foo c;
    c++++;                      // Should not be allowable.
    std::cout << c.getData();   // 1
}
