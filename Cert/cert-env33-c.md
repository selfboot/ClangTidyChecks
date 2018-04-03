C标准提供了 system() 函数，通过调用系统的命令处理程序（Unix下是shell, Win下是 CMD.EXE）来执行指定的命令。但是在程序中调用 system，可能会造成严重的安全漏洞，主要有以下几种情况：

1. 将不可信任的用户输入作为命令传递给该函数；
2. 没有指定命令的路径，同时，攻击者又可以更改命令处理程序的解析路径;
3. 被该函数执行的程序可能被攻击者篡改；
4. 制定了命令的相对路径，但是攻击者可以更改当前程序的工作目录。

下面是一个简单的示例，完整代码参考 [cert-env33-c.cpp](../Src/cert-env33-c.cpp)。

```c++
#include <iostream>
#include <string>

// Pass an unsanitized or improperly sanitized command string.
void ListDir(const std::string &params) {
    std::string ls_cmd = "ls " +  params;
    if(system(ls_cmd.c_str()) == -1) {
        std::cout << "Exec command: " << ls_cmd << std::endl;
    }
}

// A relative path to an executable is specified
void RemoveConf(){
    system("rm ~/.config");
}

int main(){
    /*
     * The shell would interpret this string as two separate commands:
     * ls any_dir;
     * echo something;
     */
    ListDir("any_dir; echo something");

    /*
     * attacker can manipulate the value of the HOME environment variable such that
     * this program can remove any file named .config anywhere on the system
     */
    // Change the HOME environment somehow.
    RemoveConf();
}
```

这里 ListDir 函数的参数可以是任意字符串，上面示例中相当于 System 调用命令处理程序执行了下面两条命令。理论上，只要权限允许，这里可以执行任意程序。

```
$ ls any_dir;
$ echo something;
```

而在 RemoveConf 函数中指定了相对路径，如果更改了 HOME 变量，那么就可以删除任意目录下的 config。

[ENV33-C. Do not call system()](https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=87152177)  


