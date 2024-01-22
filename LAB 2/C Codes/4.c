#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int pipefd[2];
    pipe(pipefd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // at 0 index we have read and 1 index we have write
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        execl("./1", "countNonAlphabetic", (char *)NULL);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execl("./2", "reverseCase", (char *)NULL);
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}

