#include <iostream>
using std::cout;
using std::endl;

void echo(int *num){
    cout << *num << endl;
}

class BaseType {};
class DerivedType: public BaseType {};
void show(DerivedType *pdt){};


typedef int (*intFuncPtr)();
double double_func(){
    cout << 3.2 << endl;
    return 3.2;
};


int main()
{
    int dividend = 3, divisor = 2;
    // 1. A C-style cast is used to convert an int to a double
    double result = ((double)dividend)/divisor;
    cout << result << endl;

    int n = 2;
    int const &ref = n;
    // 2. A C-style cast is used to remove the constness of a function parameter:
    echo((int *) &ref);

    BaseType *tmp[3];
    tmp[1] = new DerivedType;
    // 3. A C-style cast is used to convert a type in an inheritance heirarchy:
    show((DerivedType*)tmp[1]);

    // 4. A C-style cast is used to convert a double function pointer to an int function pointer
    intFuncPtr int_func = (intFuncPtr)double_func;
    int_func();

    return 0;
}


/*
double result = static_cast<double>(dividend)/divisor;                 // 1
echo(const_cast<int*>(&ref));                                          // 2
show(dynamic_cast<DerivedType*>(tmp[1]));                              // 3
IntFuncPtr int_func = reinterpret_cast<IntFuncPtr>(&double_func);      // 4
 */
