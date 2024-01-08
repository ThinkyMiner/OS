#include <stdio.h>
#include <ctype.h>

int count_non_alphabet_chars(char *input) {
    int count = 0;
    while (*input) {
        if (!isalpha(*input)) {
            count++;
        }
        input++;
    }
    return count;
}

int main() {
    char input[100];
    printf("Enter a string: ");
    fgets(input, 100, stdin);
    int count = count_non_alphabet_chars(input);
    printf("The number of non-alphabet characters in the input string is %d\n", count);
    return 0;
}
