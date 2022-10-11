/**
 * @brief pid_t fork()
 * @details
 * 使用 fork() 在 C++ 程序中创建两个进程
 *
 * @details
 * https://www.delftstack.com/howto/cpp/cpp-fork/
 *
 * @note
 * g++ process/fork/second_child.cpp -o out/child
 * g++ process/fork/second_parent.cpp -o out/parent --std=c++17
 * cd out
 * ./parent
 */
#include <atomic>
#include <filesystem>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::filesystem::exists;

constexpr int FORK_NUM = 6;

pid_t spawnChild(const char* program, char** arg_list) {
    pid_t ch_pid = fork();
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (ch_pid > 0) {
        cout << "spawn child with pid - " << ch_pid << endl;
        return ch_pid;
    } else {
        execve(program, arg_list, nullptr);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

int main() {
    string program_name("child");
    char* arg_list[] = {program_name.data(), nullptr};
    vector<int> children;
    children.reserve(FORK_NUM);

    if (!exists(program_name)) {
        cout << "Program file 'child' does not exist in current directory!\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < FORK_NUM; ++i)
        children[i] = spawnChild(program_name.c_str(), arg_list);
    cout << endl;

    pid_t child_pid;
    while ((child_pid = wait(nullptr)) > 0)
        cout << "child " << child_pid << " terminated" << endl;

    return EXIT_SUCCESS;
}
