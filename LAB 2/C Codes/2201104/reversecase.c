#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void reverseCase() {
    int c;
    while ((c = getchar()) != EOF) {
        if (islower(c)) {
            c = toupper(c);
        } else if (isupper(c)) {
            c = tolower(c);
        }
        putchar(c);
    }
}

int main(){
    reverseCase();
    return 0;
}
