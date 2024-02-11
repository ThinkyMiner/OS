#include <signal.h>
#include <setjmp.h>
jmp_buf env;
main(int argc, char *argv[]) {
signal(SIGALRM, handler);
if (setjmp(env) != 0) {
/* longjmp below causes setjmp to return here */
/* kill children ... */
/* wait loop (as below) for children ... */
exit(1);
}
/* setjmp returns here first time called */
/* open, creat, pipe, close, dup, fork 2 children ... */
alarm(15); /* set alarm for 15 seconds */
while ((pid = wait(&status)) > 0) { /* wait loop */
    fprintf(stderr, "child pid=%d reaped with exit status=%d\n",pid, WEXITSTATUS(status));
/* if pid is count's and exit status == 2, kill convert ... */
}
alarm(0); /* cancel alarm */
exit(0);
}
void handler() {
fprintf(...)
longjmp(env, 1);
exit(1);
}