/**
 * @brief pid_t fork()
 * @details
 * https://www.delftstack.com/howto/cpp/cpp-fork/
 *
 * @note
 * g++ cpp_linux/gcc/fork/fork_child.cpp -o build/child
 * g++ cpp_linux/gcc/fork/fork_parent.cpp -o build/parent
 * cd build
 * ./parent
 */
#include <filesystem>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;

pid_t spawn_child(const char* program) {
    char* argv = nullptr;

    pid_t ch_pid = fork();
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (ch_pid > 0) {
        cout << "spawn child with pid - " << ch_pid << endl;
        return ch_pid;
    }
    else {
        // 替换当前进程
        execve(program, &argv, nullptr);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    constexpr int fork_num = 6;
    const char* program_name = "child";

    std::vector<int> children;
    children.reserve(fork_num);

    if (!std::filesystem::exists(program_name)) {
        cout << "Program file 'child' does not exist in current directory!\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fork_num; ++i)
        children[i] = spawn_child(program_name);
    cout << endl;

    pid_t child_pid;
    while ((child_pid = wait(nullptr)) > 0)   // wait 等待一个进程结束
        cout << "child " << child_pid << " terminated" << endl;

    return EXIT_SUCCESS;
}
