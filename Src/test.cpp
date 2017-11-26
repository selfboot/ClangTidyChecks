#include <iostream>

typedef std::shared_ptr<int> IntPtr;

void output(const std::string& msg, int* pInt)
{
    std::cout << msg << *pInt << "\n";
}

int32_t JumpConsistentHash(uint64_t key, int32_t num_buckets) {
    int64_t b = -1, j = 0;
    while (j < num_buckets) {
        b = j;
        key = key * 2862933555777941757ULL + 1;
        j = (b + 1) * (double(1LL << 31) / double((key >> 33) + 1));
    }
    return b;
}

class Y { };
Y& operator++(Y&);     // prefix ++b
Y operator++(Y&,int); // postfix b++


int main() {
    unsigned char t= 1 & 0xf0;
    t = t>>4;
    std::cout << t << std::endl;
//    Y y;
//    y++;
//    int i=0;
//    std::cout << ++++i << std::endl;

}
