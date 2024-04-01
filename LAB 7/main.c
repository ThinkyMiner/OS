#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isSafe(int numProcesses, int numResources, int available[], int max[][numResources], int allocation[][numResources], int process){
    
    int need[numResources][numProcesses];

    for(int i = 0 ; i < numResources; i++){
        for(int j = 0; j < numProcesses; j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    
    for (int i = 0; i < numResources; i++){
        for(int j = 0 ; j < numProcesses; j++){
            if(need[i][j] < available[i]){
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    
    return 0;
}
