#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

class A
{
private:
    int a;
    int b;

public:
    A(){};
    ~A();
};

class B: public A
{
private:
    int c;
    int d;

public:
    B(){};
    ~B(){};
};

void replace(const std::string to, std::string& str) {
    std::string ss[] = {"年", "月", "-", "/"};

    std::string from;
    size_t start_pos=0;
    for(int i=0; i< sizeof(ss)/sizeof(ss[0]); i++){
        from = ss[i];
        std::cout << ss[i] <<  std::endl;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        start_pos = 0;
    }

    std::string date = "日";
    start_pos = str.find(date);
    if(start_pos == std::string::npos)
    {
        return;
    }
    str.replace(start_pos, date.length(), "");
    return;
}

int main(){

    int a = -1122;
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();

    std::cout << str << std::endl;
//    std::string birth = "";
////    int a = 0x6fd;
////    std::cout << a << std::endl;
//
//    std::string a = "1998年8月10日";
//    std::string b = "1998-8-10";
//    std::string c = "1998/8/10";
//    replace(".", c);
//    replace(".", b);
//    replace(".", a);
//    std::cout << a << std::endl << b << std::endl << c << std::endl;


//    B *b = new B();
//    delete b;
}
