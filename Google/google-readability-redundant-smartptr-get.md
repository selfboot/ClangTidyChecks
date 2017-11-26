```c++
#include <iostream>

struct Foo
{
    Foo(int in) : a(in) {}
    void print() const
    {
        std::cout << "a = " << a << '\n';
    }
    int a;
};

int main() {
    std::shared_ptr<int> i = std::shared_ptr<int>(new int(1));
    // 1.
    std::cout << *i.get() << std::endl;

    auto ptr = std::make_shared<Foo>(10);
    // 2.
    ptr.get()->print();
}
```

上面代码用到了C++11中的智能指针，其中 1，2中都是获取了被封装指针对象的地址后再进行后续操作。更简单可取的方式是直接调用智能指针的 `*` 来解引用封装的指针，或者直接用 `->` 调用封装的指针的方法。

## Smart Pointer

C++的内存管理是让很多人头疼的事，当我们写一个new语句时，一般就会立即把delete语句直接也写了，但是我们不能避免程序还未执行到delete时就跳转了或者在函数中没有执行到最后的delete语句就返回了，如果我们不在每一个可能跳转或者返回的语句前释放资源，就会造成内存泄露。使用智能指针可以很大程度上的避免这个问题，因为智能指针就是一个类，当超出了类的作用域时，类会自动调用析构函数，析构函数会自动释放资源。

C++11 废弃了 C++98 中的智能指针auto_ptr，同时引入以下3种智能指针：

* shared_ptr：资源可以被多个指针共享，它使用计数机制来表明资源被几个指针共享。
* unique_ptr：拥有唯一的对象语义的智能指针，同一对象只能被一个unique_ptr来拥有，禁止进行拷贝构造和赋值构造操作。但是，依赖于C++11中的右值引用和move语义，它可以转让所有权给另一个unique_ptr，C++11提供的move语义可以将资源unique_ptr管理的对象转移到另一个对象，但是并不进行资源的拷贝操作，只是进行资源所有权的转移，这样能够减少不必要的临时对象的创建、拷贝以及销毁，效率得到了大大的提升，当unique_ptr指针对象离开其作用域时，生命期结束，自动使用内部给定的删除器（deleter）delete所指向的对象。
* weak_ptr：是对对象的一种弱引用，不会增加对象的引用计数，和shared_ptr之间可以相互转化，shared_ptr可以直接赋值给它，它可以通过调用lock函数来获得shared_ptr。weak_ptr是用来解决shared_ptr相互引用时的死锁问题，如果说两个shared_ptr相互引用，那么这两个指针的引用计数永远不可能下降为0，资源永远不会释放。

智能指针是利用了一种叫做RAII（资源获取即初始化）的技术**对普通的指针进行封装**，这使得**智能指针实质是一个对象，行为表现的却像一个指针**。作用当然很明显，防止忘记调用delete，当然还有另一个作用就是异常安全。在一段进行了try/catch的代码段里面，即使你写入了delete，也有可能因为发生异常，程序进入catch块，从而忘记释放内存，这些都可以通过智能指针解决。

智能指针提供了 get 方法用来获取原始指针的地址，如下示例所示：

```
#include <iostream>

typedef std::shared_ptr<int> IntPtr;

void output(const std::string& msg, int* pInt)
{
    std::cout << msg << *pInt << "\n";
}

int main()
{
    int* pInt = new int(42);
    IntPtr pShared(new int(42));

    output("Naked pointer ", pInt);
    // output("Shared pointer ", pShared); // compiler error
    output("Shared pointer with get() ", pShared.get());

    delete pInt;
}
```

此外，智能指针提供了 `operator*`, `operator->`，和普通指针一样用来解引用指针，不过这里解引用的是智能智能封装的指针。所以说下面左边的语句都可以用右边表示。

```
ptr.get()->Foo()  ==>  ptr->Foo()
*ptr.get()  ==>  *ptr
*ptr->get()  ==>  **ptr
```

因此使用智能指针时，最好按照右边的格式来使用，看起来就像在使用普通指针一样。

## Ref

[How to get the Object being pointed by a shared pointer?](https://stackoverflow.com/questions/6491955/how-to-get-the-object-being-pointed-by-a-shared-pointer)   
[What is a smart pointer and when should I use one?](https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one?rq=1)  
[getting a normal ptr from shared_ptr?](https://stackoverflow.com/questions/505143/getting-a-normal-ptr-from-shared-ptr)  
[RAII and smart pointers in C++](https://stackoverflow.com/questions/395123/raii-and-smart-pointers-in-c?rq=1)  
[c++ 智能指针用法详解](http://blog.csdn.net/shuangshuang37278752/article/details/49741299)   
[如何理解智能指针？](https://www.zhihu.com/question/20368881)  


