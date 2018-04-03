#include <iostream>

using std::string;
using std::cout;
using std::endl;

class A{
public:
    const int &a;
    const string &b;
    A(const int&a, const string&b): a(a), b(b){
    }
};

void inside(const int a, const string &b){
    cout << a << b << endl;
}

int main()
{
    int a = 1;
    string b = "121";
    A demo(a, b);
    cout << demo.a << demo.b <<endl;
    inside(a, b);
    return 0;
}
