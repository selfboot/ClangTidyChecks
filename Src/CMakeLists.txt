cmake_minimum_required(VERSION 3.8)
project(ClangTidySrc)

set(CMAKE_CXX_STANDARD 99)

add_executable(demo test.cpp)
add_executable(test_2 test_2.cpp)

include_directories(Inputs)

add_executable(google-build-explicit-make-pair google-build-explicit-make-pair.cpp)
add_executable(google-build-namespaces google-build-namespaces.cpp google-namespace.cpp)
add_executable(google-build-using-namespace google-build-using-namespace.cpp)
add_executable(google-default-arguments google-default-arguments.cpp)
add_executable(google-explicit-constructor google-explicit-constructor.cpp)
add_executable(google-global-names-in-headers google-global-names-in-headers.cpp)
add_executable(google-readability-braces-around-statements google-readability-braces-around-statements.cpp)
add_executable(google-readability-casting google-readability-casting.cpp)
add_executable(google-readability-namespace-comments google-readability-namespace-comments.cpp)
add_executable(google-readability-redundant-smartptr-get google-readability-redundant-smartptr-get.cpp)
add_executable(google-runtime-int google-runtime-int.cpp)
add_executable(google-runtime-member-string-references google-runtime-member-string-references.cpp)
add_executable(google-runtime-operator google-runtime-operator.cpp)
add_executable(google-runtime-references google-runtime-references.cpp)

add_executable(cert-dcl03-c cert-dcl03-c.cpp)
add_executable(cert-dcl21-cpp cert-dcl21-cpp.cpp)
add_executable(cert-dcl54-cpp cert-dcl54-cpp.cpp)
add_executable(cert-dcl58-cpp cert-dcl58-cpp.cpp)
add_executable(cert-err52-cpp cert-err52-cpp.cpp)
add_executable(cert_err52_cpp_compliant cert_err52_cpp_compliant.cpp)
add_executable(cert-err58-cpp cert-err58-cpp.cpp)
add_executable(cert-err58-cpp_compliant cert-err58-cpp_compliant.cpp)
add_executable(cert-err60-cpp cert-err60-cpp.cpp)

add_executable(cert-err09-cpp cert-err09-cpp.cpp)

add_executable(cert-err34-c cert-err34-c.cpp)

add_executable(cert-err34-c_compliant cert-err34-c_compliant.cpp)

add_executable(cert-env33-c cert-env33-c.cpp)
