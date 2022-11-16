/**
 * @brief pid_t fork()
 * see ./second_parent.cpp
 */
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t shutdown_flag = 1;

void GracefulExit(int signal_number) {
    shutdown_flag = 0;
}

int main() {
    // Register SIGTERM handler
    signal(SIGTERM, GracefulExit);

    unsigned int tmp = 0;
    while (shutdown_flag) {
        tmp += 1;
        usleep(100);
    }

    exit(EXIT_SUCCESS);
}
