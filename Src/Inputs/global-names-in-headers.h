#ifndef CLANG_TIDY_TESTS_GLOBAL_NAMES_IN_HEADERS_H
#define CLANG_TIDY_TESTS_GLOBAL_NAMES_IN_HEADERS_H

#include <iostream>

using namespace std;
using std::cout;

namespace foo{
    using std::endl;
} // namespace foo

#endif //CLANG_TIDY_TESTS_GLOBAL_NAMES_IN_HEADERS_H



