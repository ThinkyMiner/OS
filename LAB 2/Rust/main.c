#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
    bool overlay = false;
    bool readFile = false;
    bool writeFile = false;

    int overlayIndex = 0;
    int readFileIndex = 0;
    int writeFileIndex = 0;

    argc--;
    if(argc == 3){
        overlay = true;
        readFile = true;
        writeFile = true;
        overlayIndex = 1;
        readFileIndex = 2;
        writeFileIndex = 3;
    }else if(argc == 2){
        if(argv[1][0] == '-'){
            overlay = true;
            readFile = true;
            overlayIndex = 1;
            readFileIndex = 2;
        }
        else{
            readFile = true;
            writeFile = true;
            readFileIndex = 1;
            writeFileIndex = 2;
        }
    }
    else if(argc == 1){
        readFile = true;
        readFileIndex = 1;
    }
    else{
        printf("Error: Invalid number of arguments\n");
        printf("Usage: ./main [-n] <input file> <output file>\n");
        return 1;
    }

    printf("Overlay: %d, OverlayIndex: %d\n", overlay, overlayIndex);
    printf("ReadFile: %d, ReadFileIndex: %d\n", readFile, readFileIndex);
    printf("WriteFile: %d, WriteFileIndex: %d\n", writeFile, writeFileIndex);

    




    return 0;
}
