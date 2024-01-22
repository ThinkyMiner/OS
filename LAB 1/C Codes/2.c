#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int c;
    int nonAlphabeticCount = 0;

    while ((c = getchar()) != EOF) {
        if (!isalpha(c)) {
            nonAlphabeticCount++;
        }
        putchar(c);
    }

    fprintf(stderr, "Non-alphabetic count: %d\n", nonAlphabeticCount);

    exit(0);
}
