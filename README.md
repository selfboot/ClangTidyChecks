Clang-tidy是一个基于clang的静态代码分析框架，不只可以在出现问题的地方给出提示，还提供自动修复功能（当然如何修复需要该check作者提供）。支持用户自定义check，每一个check检测一种问题，例如检测某个违反Code style的模式，检测某些API不正确的使用等。

clang-tidy 可以检测各式各样的问题：

- 检测违反代码规范的代码模式（header guard不满足，include头文件顺序错误)；
- 检测不容易在编译时发现的代码错误（把int赋值给std::string, 变量定义在头文件）；
- 纠正代码中的问题，比如把deprecated的API替换成新的API
- 把C++03的代码自动转换成C++11的代码（已经有成熟的项目这样迁移过：[electron#6423](https://github.com/electron/electron/pull/6423/files)）；

现在clang-tidy实现有100多[check](http://clang.llvm.org/extra/clang-tidy/checks/list.html)，根据check不同种类(从check名字的前缀就能知道哪一类)，分为如下几大类：

- andorid：Android相关问题 
- boost: 检测boost库API使用问题
- cert: 检测CERT的代码规范
- cpp-core-guidelines: 检测是否违反 [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Res-always)
- clang-analyzer：clang静态分析检查，参考 [Available Checkers](https://clang-analyzer.llvm.org/available_checks.html#cplusplus_checkers)
- google: 检测是否违反[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- llvm：检测是否违反llvm code style
- readability：检测不明确属于任何代码规范的可读性相关的问题
- misc：一些无法很好分类的check
- mpi：检测MPI API问题（不会用到，这里不总结了）
- modernize：倡导使用现代的语言结构（把C++03代码转换成C++11代码）
- performance：检测performance相关问题

对于所有的这些选项，均可以在Clang-tidy的[tests文件](https://github.com/llvm-mirror/clang-tools-extra/tree/master/test/clang-tidy)中，查看相应的代码演示。

上面是从规范的纬度去分类，接下来以另一个纬度将这些选项分类描述。

# 不当的函数调用 #

调用某些函数可能会导致严重的系统漏洞或者其他未定义事件，clang-tidy 可以检查源码中是否调用了高危的函数：

- cert-env33-c：系统调用函数 system(), popen(), _popen() 
- cert-err34-c：类型转换函数 atoi(), atol(), atoll(),sscanf()，atof()，主要是这些函数不能很好的处理异常
- cert-err52-cpp：setjmp()，longjmp()

某些函数的调用结果可能和预期不一致。

- google-default-arguments：检查在虚函数中使用了缺省参数值。（虚函数中使用默认参数值会表现"异常"，参见 Effective C++ 38：决不要重新定义继承而来的缺省参数值）
- misc-inaccurate-erase：检查错误的 `erase()` 函数调用。

# 错误（可疑）的用法 #

C++ 中有些符合语法规则的代码并不能正常运行，可能编译失败或者出现运行时错误，clang-tidy 会检测相应的错误语句：

- cert-fio38-c（misc-non-copyable-objects）：对不能进行值传递的对象进行解引用和非指针声明，比如 C 中的FILE对象或者是POSIX中的pthread_mutex_t
- cert-flp30-c：循环语句中，归纳表达式中有 float 类型。
- google-build-explicit-make-pair：检查在使用 `make_pair` 时显式地指明参数类型。这个问题可以参考：[C++11 make_pair with specified template parameters doesn't compile](https://stackoverflow.com/questions/9641960/c11-make-pair-with-specified-template-parameters-doesnt-compile)
- misc-assert-side-effect：找出所有有副作用的 `assert()` 调用。（assert调用发生在debug模式下，如果有副作用，会导致debug和run可能有不同结果）
- misc-incorrect-roundings：找出所有已知的不正确的四舍五入。
- misc-macro-parentheses:查找由于缺少括号而导致意外行为的宏。
- misc-macro-repeated-side-effects:检查宏中带有副作用的重复的参数。
- misc-misplaced-widening-cast：检查将计算结果转换为较大类型的 cast 操作。
- misc-move-const-arg：检查错误的 `std::move()` 调用，主要有3种情况：1. 参数是常量；2. 参数是 trivially-copyable type；3. 函数结果作为 const 引用被传递。
- misc-multiple-statement-macro：检查在省略大括号的条件语句中使用有多条语句的宏。
- misc-sizeof-expression：检查很可能是错误的 `sizeof运算符` 使用。
- misc-string-constructor：检查可疑地或者错误地string构造方法调用。
- misc-string-integer-assignment：检查将整数类型赋值给std::basic_string<CharT> (std::string, std::wstring）等的操作。
- misc-string-literal-with-embedded-nul：检查程序中使用嵌有NUL字符的字符串字面量。
- misc-suspicious-enum-usage：检测 enum 类型可能被滥用的情况。
- misc-suspicious-missing-comma：检查可能缺少逗号导致字符串字面量进行了拼接。
- misc-suspicious-semicolon：检查大部分情况下会改变代码语义的不恰当的分号。
- misc-use-after-move：检查在 `std::move()` 后没有重新初始化就使用对象。
- misc-unused-raii：查找可能是RAII对象的临时变量（临时对象创建后就被销毁，不能发挥作用）。
- bugprone-integer-division：检查浮点上下文中的整数除法可能导致意外精度损失的情况。
- bugprone-suspicious-memset-usage：检查调用 `memset` 参数中存在潜在错误的情况。
- bugprone-undefined-memory-manipulation：检查调用memset(), memcpy() 和 memmove()时，对象不是 `TriviallyCopyable objects`。（如果这样做，将导致`undefined behavior`）
- cert-dcl21-cpp：如果返回类型不是const对象，或者是一个引用类型，此时进行后缀操作符++和-- 的声明是不恰当的。

# 内存分配相关 #

动态内存的分配与回收是C++中很容易犯错误的地方，内存泄漏问题屡见不鲜。

- cert-dcl54-cpp（hicpp-new-delete-operators，misc-new-delete-overloads）：检查没有在同一个作用域内同时重载operator new() 和 operator delete()。
- cppcoreguidelines-no-malloc：检测动态分配内存malloc和释放内存free函数的使用；

# 内存访问越界 #

- cppcoreguidelines-pro-bounds-array-to-pointer-decay：检测代码中将数组退化到指针的操作。
- cppcoreguidelines-pro-bounds-constant-array-index：检查数组索引中非常量表达式的使用。
- cppcoreguidelines-pro-bounds-pointer-arithmetic：检查代码中对指针进行算数操作（不检查两个指针相减这种情况）
- misc-dangling-handle：检查使用 std::experimental::string_view 这类句柄导致的引用悬挂（dangling references）问题。

# 类型安全 #

关于类型安全，一个不错的[解释](http://www.cs.cornell.edu/courses/cs1130/2012sp/1130selfpaced/module1/module1part4/strongtyping.html)是：

> Safety: Any attempt to misinterpret data is caught at compile time or generates a well-specified error at runtime.

可以理解为类型系统给内存中的data赋予了类型信息，如果不按照类型信息来解释内存中的数据，应该产生编译错误或者 产生well-specified的运行时错误。 类型安全会避免下列错误的发生：

 - 非法的操作。比如整型拿去给字符串除，显然不符合逻辑。比如一个指向一个整数的指针，拿去当数组名访问元素，肯定也会出错。这类非法的操作是类型安全语言最基本的要求。
 - 保证类型使用的安全性。类型都有自己的内存布局，都有自己的大小。使用错误的类型去解释一块内存，或者去做复制操作，读写操作都可能造成意料之外的错误。这样的不安全内存操作应该规避。

以下的 Flag 会检查可能破坏类型安全的地方（unions、casts、array decay、range errors、narrowing conversions）：

- cppcoreguidelines-pro-type-cstyle-cast：检查代码中用`static_cast`进行向下转型，或者使用了`const_cast`,或者是`reinterpret_cast`.
- cppcoreguidelines-pro-type-const-cast：检查代码中用 `const_cast`将 const 强制转换为 non const.
- cppcoreguidelines-pro-type-static-cast-downcast：检查代码中用`static_cast`进行向下转型.
- cppcoreguidelines-pro-type-reinterpret-cast：检查代码中用`reinterpret_cast`进行类型转换.
- cppcoreguidelines-pro-type-member-init：检查用户定义的构造函数中，没有对不会默认进行初始化的字段进行初始化操作。
- cppcoreguidelines-pro-type-union-access：检查程序中对于union成员的访问。
- cppcoreguidelines-pro-type-vararg：检查C风格的使用不定长参数的函数，以及所有使用va_arg的地方。
- [x] misc-fold-init-type：检查在使用比如 std::accumulate 这些 [folds](https://en.wikipedia.org/wiki/Fold_%28higher-order_function%29) 函数时，参数类型不匹配导致的精度丢失问题。

# Scope (作用域) #

- cert-dcl58-cpp：检查修改std或posix命名空间的行为，因为很可能导致`undefined behavior`。
- cert-dcl59-cpp（google-build-namespaces）：检查在头文件中有匿名命名空间的问题；
- google-build-using-namespace:检查代码中使用 `using namespace` 的情况；
- google-global-names-in-headers：检查头文件中使用 `using` 可能导致的命名空间污染问题；
- misc-definitions-in-headers：检查头文件中`non-extern`、`non-inline`的函数和变量定义，因为多个编译单元包含头文件时，可能会导致潜在的 [ODR](https://en.wikipedia.org/wiki/One_Definition_Rule) 问题。
- misc-forward-declaration-namespace：找出所有未使用的前置变量声明，并检查是否有相同名字的声明或者定义（未使用的变量声明可能在错误的命名空间）。
- misc-unused-alias-decls：查找未使用的命名空间别名声明。

# 类相关问题 #

- google-explicit-constructor：检查在单参数构造函数（single-argument constructors）中，没有使用 `explicit` 来避免可能导致的隐式类型转换。
- [x] cppcoreguidelines-slicing：检查派生类转换为基类时发生的slicing问题。
- misc-forwarding-reference-overload：检查前置构造函数覆盖掉拷贝构造或者移动构造函数的情况。
- cert-oop11-cpp（misc-move-constructor-init）：检查用户定义的移动构造函数，其通过复制构造函数而不是移动构造函数初始化成员或基类。
- cppcoreguidelines-c-copy-assignment-signature（misc-unconventional-assign-operator）：检查声明赋值操作符时参数类型或者返回值错误，定义赋值操作符时返回语句错误。
- misc-virtual-near-miss：检查派生类函数和基类的虚函数十分接近的情况（名字类似，并且有同样的函数签名）。
- modernize-pass-by-value：构造函数中尽量使用std::move而不是const reference 来传递参数。
- cert-err60-cpp：检查所有的异常抛出语句中，异常对象不是`nothrow copy constructible`。

# 可读性 #

代码应该保持简介，清晰明了。多余的语句或者不当的缩进、函数调用等都将影响代码的可读性，下面列出一些可以提高可读性的改进措施：

- google-readability-braces-around-statements(readability-braces-around-statements)：检查if语句和循环（for，do while和while）的内容是否在大括号内。
- google-readability-casting：检查代码中 C 风格的类型转换（建议使用更易读的C++风格进行类型转换）。
- google-readability-function-size（readability-function-size）: 用一系列标准检查比较臃肿的函数。（比如行数、语句数，分支数） 
- google-readability-namespace-comments（llvm-namespace-comment）：检查长的命名空间（namespace）后面跟有注释。
- google-readability-redundant-smartptr-get（readability-redundant-smartptr-get）：检查并删除使用指针时多余的`.get()`方法调用。
- google-readability-todo：检查todo注释中没有指明bug名字或者负责人的；
- misc-argument-comment：检查使用 `/*parameter_name=*/` 注释参数时注释名字和参数名字是否匹配。
- readability-avoid-const-params-in-decls：去掉函数声明中多余的 `top const`（声明中top const无效，函数定义中才会用到）。
- readability-container-size-empty：检查容器是否为空时，应该使用`empty()`，而不是`size()`函数。
- readability-delete-null-pointer：检查在 `delete ptr` 前，用`if(ptr)`判断ptr是否为空（删除nullptr不会导致错误）。
- readability-deleted-default：检查虽然用 `=default` 定义了默认构造或赋值函数，编译器却删除了这些函数。
- readability-else-after-return：当使用`return, break, continue, throw`打断正常的控制流程时，不要在后面使用 `else if` 或者 `else`。
- readability-identifier-naming：检查不符合规定的标志符命名。
- readability-implicit-bool-conversion：检查内置类型和bool类型之间的隐式类型转换。
- readability-inconsistent-declaration-parameter-name：检查同一个函数在不同地方进行声明时，参数名字不一致。
- readability-misleading-indentation：检查错误的缩进（可能对理解代码造成麻烦）。语法结构的不匹配和代码的缩进可能会隐藏严重的问题，因此正确的缩进和大括号的使用很重要。
- readability-misplaced-array-index：检查异常的数组索引语法。
- readability-non-const-parameter：检查指针作为函数参数时，可以将其改为指向常量的指针（point to constant）的情况。
- readability-redundant-control-flow：检查冗余的return或者continue。
- readability-redundant-declaration：检查重复的函数或者变量声明。
- readability-redundant-function-ptr-dereference：检查冗余的函数指针解引用操作。（[Why do function pointer definitions work with any number of ampersands '&' or asterisks '*'?](https://stackoverflow.com/questions/6893285/why-do-function-pointer-definitions-work-with-any-number-of-ampersands-or-as)）
- readability-redundant-member-init：如果构造函数有初始化成员变量，那么额外的成员变量初始化就显得冗余。
- readability-redundant-string-cstr：检查不必要的 `std::string::c_str()` 和 `std::string::data()`调用。
- readability-redundant-string-init：检查不必要的string初始化。（个人感觉显式初始化比默认初始化更好）
- readability-simplify-boolean-expr：查找涉及布尔常量的布尔表达式，并直接使用适当的布尔表达式简化它们。
- readability-static-accessed-through-instance：检查通过实例访问静态成员的成员表达式，并使用类访问替换。
- readability-static-definition-in-anonymous-namespace：检查在匿名命名空间用 static 定义变量或者函数。
- readability-uniqueptr-delete-release：用`<unique_ptr> = nullptr`替换`delete <unique_ptr>.release()`。 

# 更好的写法

代码中可能有冗余的地方，或者用了不恰当（但正确）的函数，或者是某些用法在新的标准中已经被弃用，或者新的标准有了更好的写法，下面给出一系列可以对代码进行**升级**的地方。

- boost-use-to-string：将整型转换为 string 或者 wstring 类型时，用`std::to_string`和`std::to_wstring`，而不是`boost::lexical_cast`。
- misc-string-compare：找出用`compare`方法来做string类型的比对（使用 `==` 或者 `!=` 来替换）。
- misc-suspicious-string-compare：检查可疑地使用strcmp方法。
- misc-uniqueptr-reset-release：用`std::move()`替换`unique_ptr::reset(release())`来实现转移语义。
- misc-unused-parameters：查找并修复未使用的参数。
- misc-unused-using-decls：查找没有使用的 `using` 声明语句。
- modernize-avoid-bind：用 lambda 函数替换 `std::bind`。
- modernize-deprecated-headers：用C++头文件替换C标准库头，并删除冗余的C头文件。
- modernize-loop-convert：用 C++11 的 range-based 循环替换 `for(...; ...; ...) ` 格式的循环语句
- modernize-make-shared：用`std::make_shared` 替换用 std::shared\_ptr 构造函数和 new 来创建shared\_ptr；
- modernize-make-unique：用`std::make_unique` 替换用 std::unique\_ptr 构造函数和 new 来创建unique\_ptr；
- modernize-raw-string-literal：有选择地使用`raw string literals` 替换包含转义字符的字符串文字。
- modernize-redundant-void-arg：查找并删除冗余的void参数列表。
- modernize-replace-auto-ptr：用`std::unique_ptr`（在C++ 11中引入）替换已弃用的`std::auto_ptr`。
- modernize-replace-random-shuffle：用`std::shuffle`替换`std::random_shuffle`。
- modernize-return-braced-init-list：在返回语句中用初始化列表来替换对构造函数的显式调用。
- modernize-shrink-to-fit：使用`shrink_to_fit()`方法调用替换可收缩容器上的复制和交换操作。
- modernize-unary-static-assert：找出具有空字符串的`static_assert`声明，并使用单参数static_assert声明替换(C++ 17)。
- modernize-use-auto：使用类型推导符 auto 来进行变量声明， 以提高代码的可读性和可维护性。
- modernize-use-bool-literals：将整型转换为 bool 类型。
- modernize-use-default-member-init：将默认构造函数成员初始化列表转换为C ++ 11中新的默认成员初始化操作。
- modernize-use-emplace：在容器std::vector, std::deque, std::list中用更加高效的 `emplace_back` 替换 `push_back` 调用。（详情可以参考[push_back vs emplace_back](https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back)）
- modernize-use-noexcept：此检查将使用适当的`noexcept`规范（在C ++ 11中引入）替换不推荐使用的动态异常规范。（详情可以参考[Deprecated throw-list in C++11](https://stackoverflow.com/questions/13841559/deprecated-throw-list-in-c11)）
- modernize-use-nullptr：使用新的C++11 nullptr关键字替换空指针常量（NULL，0）。(详情可以参考：[What exactly is nullptr?](https://stackoverflow.com/questions/1282295/what-exactly-is-nullptr))
- hicpp-use-override（modernize-use-override）：使用 `override` 来声明重载函数。
- modernize-use-transparent-functors：尽量使用 `transparent functors` 而不是 `non-transparent functors`。
- modernize-use-using：用C++11 的 `using` 关键字替换`typedef`。

# 性能相关

有些写法可能导致性能问题，可以用更高效的写法来替换。

- performance-faster-string-find：当std::string::find()的参数是单个字符时，使用用字符而不是字符串（`string.find("a"); -> string.find('a');`）。
- performance-for-range-copy：如果循环变量的复制操作过于复杂，或者使用自定义的拷贝构造函数，clang-tity会添加const reference到这些变量以保证系统的速度和安全性
- performance-implicit-conversion-in-loop：检测基于范围的循环中，当循环变量为const ref类型时，变量的类型与迭代器返回的变量不匹配的情况。
- performance-inefficient-string-concatenation：使用`append()`而不是+进行字符串连接。
- performance-inefficient-vector-operation：找到可能导致不必要的内存重新分配的低效的std::vector操作（push_back，emplace_back等）。
- performance-type-promotion-in-math-fn：使用math库的函数（参数类型和返回参数都是double）时，传递float参数，导致了隐式类型转换。可以使用 std 命名空间下的数学函数。
- performance-unnecessary-copy-initialization：本地变量尽量用const reference，避免没必要的拷贝构造函数调用。
- performance-unnecessary-value-param：函数参数的拷贝构造函数代价比较大时，尽量用const reference来传参。

# 其他

- cert-err58-cpp：检查在static或thread_local变量声明时进行初始化操作可能会引发异常的情况。
- cert-dcl50-cpp：检查在函数定义时，参数用C风格的可变参。
- cert-msc30-c（cert-msc50-cpp）：`std::rand()`函数接收一个种子（数），对其运行一个数学运算并返回结果。通过操纵种子，可以预测结果，此检查警告std::rand()的使用。
* google-runtime-int：检查并用 `u?intXX(_t)?` 替换代码中的short, long和long long。
* google-runtime-member-string-references：检查类型为const string&的类成员，const string& 容易导致悬挂引用（参考 [Returning const reference](https://stackoverflow.com/questions/36673425/returning-const-reference)）。
* google-runtime-operator：检查代码中unary operator &（一元操作符&）的重载。
* google-runtime-references：检查包含有non-constant引用参数的函数。C++中，所有引用传递的参数必须设置为 const 引用。
* cert-dcl03-c（misc-static-assert）：如果语句在编译时可确定，则用`static_assert()`替换`assert()`。 
- cppcoreguidelines-special-member-functions：检查只使用部分编译器生成的默认方法（copy constructor, copy assignment operator, move constructor, move assignment operator and destructor）的类。
- misc-bool-pointer-implicit-conversion：检查在条件判断语句中存在bool pointer到 bool的隐式类型转换；
- misc-misplaced-const：检查 `typedef` 定义指针后，用 const 修饰该指针。
- misc-sizeof-container：检查对STL容器类型使用sizeof运算符。
- hicpp-no-assembler：检查代码中用`asm ( string_literal ) `使用 inline assembly，因为这样不方便移植。
- misc-inefficient-algorithm：检查对关联容器使用低效的STL算法（应该用容器自带的高效算法）；
- misc-lambda-function-name：检查尝试从lambda表达式中获取函数的名称；
- misc-move-forwarding-reference：检查在 `forwarding reference` 时调用 `std::move` 函数，参考[移动语义（move semantic）和完美转发（perfect forward）](https://codinfox.github.io/dev/2014/06/03/move-semantic-perfect-forward/)。
- misc-redundant-expression：检查通常是因为复制粘贴不小心导致的冗余的表达式。
- misc-swapped-arguments：通过查看隐式转换来查找潜在的交换参数。
- cert-err09-cpp（cert-err61-cpp，misc-throw-by-value-catch-by-reference）：检查违背“`Throw by value, catch by reference`” 规则的代码。
* hicpp-exception-baseclass：保证thorw语句抛出的每个值都是std::exception的实例。
* hicpp-invalid-access-moved（misc-use-after-move）：检查代码中局部对象或者函数参数被 `std::move` 之后重新使用的地方。
* hicpp-named-parameter（readability-named-parameter）：检查函数中是否有未命名参数
- hicpp-undelegated-constructor（misc-undelegated-constructor）：检查在构造函数中调用同一个类的另一个构造函数创建了临时对象（原意可能是调用委托构造函数或者基类的构造函数）。
- hicpp-use-equals-default (modernize-use-equals-default)：用 `= default` 替换编译器会默认生成的函数的默认函数体。
- hicpp-use-equals-delete（modernize-use-equals-delete）：用 `= delete`来指定所有没有实现的 private 编译器默认生成的函数。
- hicpp-noexcept-move（misc-noexcept-move-constructor）：检查用户自定义移动构造函数和移动赋值操作符时没有指明 `noexcept`。
- ~~hicpp-special-member-functions （cppcoreguidelines-special-member-functions）~~
* ~~hicpp-explicit-conversions~~
* ~~hicpp-function-size~~
* ~~hicpp-braces-around-statements~~
* ~~hicpp-member-init~~
- llvm-header-guard：查找并修复不遵守LLVM风格的header保护声明（[What is proper LLVM header guard style?](https://stackoverflow.com/questions/43880907/what-is-proper-llvm-header-guard-style)）。
- llvm-include-order：检查 `#include`的正确顺序。
- ~~llvm-namespace-comment（google-readability-namespace-comments）~~
- llvm-twine-local：检查局部 `Twine` 的使用。


