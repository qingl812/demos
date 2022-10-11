#include <errno.h>   // errno
#include <fcntl.h>   // O_CREAT
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>
#include <sys/stat.h>   // mkfifo
#include <sys/wait.h>
#include <unistd.h>   // read

/***
 * @brief linux c pipe
 * @include unistd.h
 * @ref int pipe(int pipefd[2]);
 *
 * @details
 * pipe() 创建一个管道，一个可用于进程间通信的单向数据通道。
 * 数组 pipefd 用于返回两个引用管道末端的文件描述符。
 * pipefd[0] 指的是管道的读取端。 pipefd[1] 指的是管道的写端。
 * 写入管道写入端的数据由内核缓冲，直到从管道的读取端读取。
 * 如果 flags 为 0，则 pipe2() 与 pipe() 相同。
 *
 * @return
 * 成功时，返回零。 出错时，返回 -1，并适当设置 errno。
 *
 * @details
 * https://linux.die.net/man/2/pipe
 */
TEST(pipe_case, anonymous_pipe) {
    const char* str = "hello anonymous pipe";

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (cpid > 0) {
        // Parent writes str to pipe
        close(pipefd[0]); /* Close unused read end */
        write(pipefd[1], str, strlen(str));
        close(pipefd[1]); /* Reader will see EOF */
        wait(nullptr);    /* Wait for child */
    }
    else {
        char buf;

        // Child reads from pipe
        close(pipefd[1]); /* Close unused write end */
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
}

/***
 * @brief 命名管道 mkfifo
 * @include sys/stat.h
 * @ref int mkfifo(const char* pathname, mode_t mode);
 * @return 成功，0；失败，-1
 * @details
 * 本程序从一个FIFO读数据，并把读到的数据打印到标准输出
 * 如果读到字符 Q ，则退出
 * https://www.cnblogs.com/xianghang123/archive/2012/03/31/2427731.html
 * https://man7.org/linux/man-pages/man2/umask.2.html
 *
 * @note
 * rm /tmp/my_fifo
 */
TEST(pipe_case, named_pipe) {
    const char* fifo_path = "/tmp/my_fifo";
    const char* str = "hello named pipe";

    if ((mkfifo(fifo_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) &&
        (errno != EEXIST)) {
        printf("不能创建FIFO\n");
        exit(1);
    }

    // 创建子进程写入数据
    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (cpid > 0) {
        int fd = open(fifo_path, O_RDONLY, 0);
        if (fd == -1) {
            perror("打开FIFO");
            exit(1);
        }

        char buf;
        printf("read from named pipe：");
        while (read(fd, &buf, 1) > 0) {
            if (buf == 'Q')
                break;
            else
                printf("%c", buf);
        }
        printf("\n");
        close(fd);
    }
    else {
        int fd = open(fifo_path, O_WRONLY, 0);
        if (fd == -1) {
            perror("打开FIFO");
            exit(1);
        }

        if (write(fd, str, strlen(str)) == -1) {
            printf("写入错误\n");
            ASSERT_TRUE(false);
        }
        if (write(fd, "Q", 2) == -1) {
            printf("写入错误\n");
            ASSERT_TRUE(false);
        }

        close(fd);
        exit(EXIT_SUCCESS);
    }
}
