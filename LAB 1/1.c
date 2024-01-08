#include <stdio.h>

int main(int args, char* words[]){
    int count  = 1;
    while (count < args){
        if(words[count][0] != '-'){
            printf("%s", words[count]);
        }
        count++;
    }
}