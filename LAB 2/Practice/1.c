#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char const *argv[]){
    for (int i = 0; i < 3; i++)
    {
        /* code */
        fork();
    }
    
    printf("Hello\n");
    return 0;
}
