#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 100
#define MAX_NAME_LENGTH 50
typedef struct {
  char name[MAX_NAME_LENGTH];
  int arrival;
  int cpuburst;
  int turnaround;
  int remainingBurst;
  int wait;
} Process;

int global = 0;

Process processTable[MAX_PROCESS];

int current_time;

int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

void ReadProcessTable(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error: Could not open file %s\n", filename);
    return;
  }

  int i = 0;
  while (fscanf(file, "%s %d %d", processTable[i].name,
                &processTable[i].arrival, &processTable[i].cpuburst) == 3) {
    i++;
  }

  global = i;

  fclose(file);

  // Print only if processes were successfully read
  if (i > 0) {
    printf("Read processes:\n");
    for (int j = 0; j < i; j++) {
      printf("Process ID: %s, Arrival Time: %d, Burst Time: %d\n",
             processTable[j].name, processTable[j].arrival,
             processTable[j].cpuburst);
    }
  } else {
    printf("No processes found in the file.\n");
  }
}

void FCFS() {
  int currentTime = 0;
  for (int i = 0; i < global; i++) {
    printf("[%d - %d] %s running\n", currentTime,
           currentTime + processTable[i].cpuburst, processTable[i].name);
    currentTime = currentTime + processTable[i].cpuburst;
  }
  printf("Turn Around Time\n");
  int TAsum = 0;
  currentTime = 0;
  for (int i = 0; i < global; i++) {
    TAsum += currentTime + processTable[i].cpuburst - processTable[i].arrival;
    currentTime += processTable[i].cpuburst;
    printf("%s [%d]\n", processTable[i].name,
           currentTime - processTable[i].arrival);
  }
  printf("Wait Time\n");
  int Wsum = 0;
  currentTime = 0;
  printf("%s [%d]\n", processTable[0].name, 0);
  for (int i = 1; i < global; i++) {
    currentTime += processTable[i - 1].cpuburst;
    Wsum += currentTime - processTable[i].arrival;
    printf("%s [%d]\n", processTable[i].name,
           currentTime - processTable[i].arrival);
  }
  printf("Average Turn Around Time : %d \nAverage Waiting Time : %d\n",
         TAsum / (global), Wsum / (global));
}

void RoundRobin() {
  int index = global;
  int timeQuantum = 0;
  printf("Enter the time quantum : ");
  scanf("%d", &timeQuantum);
  int currentTime = 0;
  Process tempTable[MAX_PROCESS];
  int j = 0;
  int count = 1;
  int remainingTime[global];
  for (int i = 0; i < global; i++) {
    remainingTime[i] = processTable[i].cpuburst;
  }
  while (count < index) {
    for (int i = 0; i < index; i++) {
      if (processTable[i].cpuburst > 0 &&
          currentTime >= processTable[i].arrival) {
        printf("[%d - %d] %s running\n", currentTime,
               min(currentTime + timeQuantum,
                   currentTime + processTable[i].cpuburst),
               processTable[i].name);

        currentTime = min(currentTime + timeQuantum,
                          currentTime + processTable[i].cpuburst);
        processTable[i].cpuburst -= timeQuantum;
        processTable[i].turnaround = currentTime - processTable[i].arrival;
        processTable[i].wait =
            currentTime - processTable[i].arrival - remainingTime[i];
        if (processTable[i].cpuburst <= 0) {
          count++;
        }
      }
    }
  }
  int TAsum = 0;
  printf("Turn Around Time\n");
  for (int i = 0; i < global; i++) {
    printf("%s [%d]\n", processTable[i].name, processTable[i].turnaround);
    TAsum += processTable[i].turnaround;
  }
  
  int Wsum = 0;
  printf("Wait Time\n");
  for (int i = 0; i < global; i++) {
    printf("%s [%d]\n", processTable[i].name, processTable[i].wait);
    Wsum += processTable[i].wait;
  }
  printf("Average Turn Around Time : %d\n", TAsum / (global));
  printf("Average Waiting Time : %d\n", Wsum / (global));
}

void SRBF(int global) {
    int currentTime = 0;
    int index = global;

    for (int i = 0; i < global; i++) {
        processTable[i].remainingBurst = processTable[i].cpuburst;
    }

    while (index > 0) {
        int minIndex = -1;

        for (int i = 0; i < global; i++) {
            if (processTable[i].arrival <= currentTime &&
                processTable[i].remainingBurst > 0 &&
                (minIndex == -1 || processTable[i].remainingBurst < processTable[minIndex].remainingBurst)) {
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
        } else {
            printf("[%d - %d] %s running\n", currentTime,
                   currentTime + processTable[minIndex].remainingBurst,
                   processTable[minIndex].name);

            currentTime += processTable[minIndex].remainingBurst;
            processTable[minIndex].remainingBurst = 0;
            processTable[minIndex].turnaround = currentTime - processTable[minIndex].arrival;
            processTable[minIndex].wait = currentTime - processTable[minIndex].arrival - processTable[minIndex].cpuburst;

            index--;
        }
    }
    int TAsum = 0;
    printf("Turnaround Time\n");
    for (int i = 0; i < global; i++) {
        printf("%s [%d]\n", processTable[i].name, processTable[i].turnaround);
        TAsum += processTable[i].turnaround;
    }
    
    int Wsum = 0;
    printf("Waiting Time\n");
    for (int i = 0; i < global; i++) {
        printf("%s [%d]\n", processTable[i].name, processTable[i].wait);
        Wsum += processTable[i].wait;
    }
    printf("Average Turnaround Time : %d\n", TAsum / (global));
    printf("Average Waiting Time : %d\n", Wsum / (global));
}



int main() {
  int i = -1;
  printf("Select the file name from 1.txt, 2.txt, 3.txt, 4.txt: ");
      char filename[100];
      scanf("%s", filename);
  while(i){
    printf("Enter 1 to run FCFS\n");
    printf("Enter 2 to run Round Robin\n");
    printf("Enter 3 to run SRBF\n");
    printf("Enter 4 to change the file\n");
    printf("Enter 0 to exit\n");
    scanf("%d", &i);
    if(i == 1){
      ReadProcessTable(filename);
      FCFS();
    }
    else if(i == 2){
      ReadProcessTable(filename);
      RoundRobin();
    }
    else if(i == 3){
      ReadProcessTable(filename);
      SRBF(global);
    }
    else if(i == 4){
      printf("Enter the file name (1.txt / 2.txt / 3.txt / 4.txt): ");
      char filename[100];
      scanf("%s", filename);
    }

  }
  return 0;
}
