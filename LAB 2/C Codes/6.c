#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd_in, fd_out;
    int pipe_fd[2];
    pid_t pid1, pid2;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        exit(1);
    }

    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("Error opening file1");
        exit(1);
    }

    fd_out = creat(argv[2], 0644);
    if (fd_out == -1) {
        perror("Error creating file2");
        exit(1);
    }

    
    dup2(fd_in, 0);
    close(fd_in);

   
    dup2(fd_out, 1);
    close(fd_out);

   
    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        exit(1);
    }

    if ((pid1 = fork()) == 0) {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], 0);
        close(pipe_fd[0]);

        int fd_err = creat(argv[2], 0644);
        dup2(fd_err, 2);
        close(fd_err);

        execl("count", "count", (char *)0);
        perror("Error executing count");
        exit(1);
    }

    if ((pid2 = fork()) == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        close(pipe_fd[1]);
        execl("convert", "convert", (char *)0);
        perror("Error executing convert");
        exit(1);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    wait(NULL);
    wait(NULL);

    exit(0);
}