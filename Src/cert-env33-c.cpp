#include <iostream>

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
