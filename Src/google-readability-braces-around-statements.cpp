#include <iostream>

int main(){
    using std::cout;
    using std::endl;

    int foo = 1;
    if(foo > 2)                     // 1
        cout << "If" << endl;

    for(int i=0; i<1; i++)          // 2
        cout << i << endl;

    while(foo++ < 2)                // 3
        cout << foo << endl;
}
