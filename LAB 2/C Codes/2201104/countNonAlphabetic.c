#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void countNonAlphabetic() {
    int c;
    int nonAlphabeticCount = 0;
    while ((c = getchar()) != EOF) {
        if (!isalpha(c)) {
            nonAlphabeticCount++;
        }
        putchar(c);
    }

    fprintf(stderr, "Number of non-alphabetic characters: %d\n", nonAlphabeticCount);
}

int main(){
    countNonAlphabetic();
    return 0;
}