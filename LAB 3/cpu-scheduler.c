#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 100

typedef struct {
    char * name;
    int arrival;
    int cpuburst;
    int turnaround;
    int wait;
} Process;

Process processtable[MAX_PROCESS];
int current_time;

void ReadProcessTable(char * filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return;
    }

    int i = 0;
    while (!feof(file)) {
        processtable[i].name = malloc(50 * sizeof(char));
        fscanf(file, "%s %d %d", processtable[i].name, &processtable[i].arrival, &processtable[i].cpuburst);
        i++;
    }

    fclose(file);
}

void PrintProcessTable() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        printf("%s %d %d\n", processtable[i].name, processtable[i].arrival, processtable[i].cpuburst);
    }
}

void FCFS() {
    current_time = 0;
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processtable[i].arrival > current_time) {
            processtable[i].wait = processtable[i].arrival - current_time;
            current_time = processtable[i].arrival;
        } else {
            processtable[i].wait = 0;
        }
        current_time += processtable[i].cpuburst;
        processtable[i].turnaround = current_time - processtable[i].arrival;
    }
}

void PrintStatistics() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        printf("%s %d %d\n", processtable[i].name, processtable[i].turnaround, processtable[i].wait);
    }
}

void RR(int quantum) {
    current_time = 0;
    int remaining = MAX_PROCESS;
    while (remaining > 0) {
        for (int i = 0; i < MAX_PROCESS; i++) {
            if (processtable[i].arrival <= current_time && processtable[i].cpuburst > 0) {
                if (processtable[i].cpuburst > quantum) {
                    current_time += quantum;
                    processtable[i].cpuburst -= quantum;
                } else {
                    current_time += processtable[i].cpuburst;
                    processtable[i].cpuburst = 0;
                    remaining--;
                    processtable[i].turnaround = current_time - processtable[i].arrival;
                }
            }
        }
    }
}

void SRBF() {
    current_time = 0;
    int remaining = MAX_PROCESS;
    while (remaining > 0) {
        int min = 1000000;
        int index = -1;
        for (int i = 0; i < MAX_PROCESS; i++) {
            if (processtable[i].arrival <= current_time && processtable[i].cpuburst < min && processtable[i].cpuburst > 0) {
                min = processtable[i].cpuburst;
                index = i;
            }
        }
        if (index == -1) {
            current_time++;
        } else {
            processtable[index].cpuburst--;
            current_time++;
            if (processtable[index].cpuburst == 0) {
                remaining--;
                processtable[index].turnaround = current_time - processtable[index].arrival;
            }
        }
    }
}

int main() {
    ReadProcessTable("processes.txt");

    FCFS();
    printf("After FCFS:\n");
    PrintProcessTable();
    PrintStatistics();

    RR(10);
    printf("After RR:\n");
    PrintProcessTable();
    PrintStatistics();

    SRBF();
    printf("After SRBF:\n");
    PrintProcessTable();
    PrintStatistics();

    return 0;
}