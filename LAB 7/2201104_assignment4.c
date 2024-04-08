#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define MAX_RESOURCES 100

bool is_safe(int num_processes, int num_resources, int *available,
             int allocation[MAX_PROCESSES][MAX_RESOURCES],
             int need[MAX_PROCESSES][MAX_RESOURCES]) {
  int work[MAX_RESOURCES];
  bool finish[MAX_PROCESSES] = {false};
  int sequence[MAX_PROCESSES];
  int count = 0;

  // Initialize work to available resources
  for (int i = 0; i < num_resources; i++) {
    work[i] = available[i];
  }

  // Safety algorithm
  while (true) {
    bool found = false;
    for (int i = 0; i < num_processes; i++) {
      if (!finish[i]) {
        bool can_allocate = true;
        for (int j = 0; j < num_resources; j++) {
          if (need[i][j] > work[j]) {
            can_allocate = false;
            break;
          }
        }
        if (can_allocate) {
          for (int j = 0; j < num_resources; j++) {
            work[j] += allocation[i][j];
          }
          finish[i] = true;
          sequence[count++] = i;
          found = true;
        }
      }
    }
    // If no process can be finished, break the loop
    if (!found) {
      break;
    }
  }

  // If all processes finish, the system is in a safe state
  for (int i = 0; i < num_processes; i++) {
    if (!finish[i]) {
      return false;
    }
  }
  return true;
}

int main() {
  int num_resources, num_processes;
  int resource_instances[MAX_RESOURCES];
  int allocation[MAX_PROCESSES][MAX_RESOURCES];
  int request[MAX_PROCESSES][MAX_RESOURCES];

  printf("Enter the number of distinct resources: ");
  scanf("%d", &num_resources);

  printf("Enter the number of instances of each resource type: ");
  for (int i = 0; i < num_resources; i++) {
    scanf("%d", &resource_instances[i]);
  }

  printf("Enter the number of processes: ");
  scanf("%d", &num_processes);

  printf("Enter the allocation matrix:\n");
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      scanf("%d", &allocation[i][j]);
    }
  }

  printf("Enter the request matrix:\n");
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      scanf("%d", &request[i][j]);
    }
  }

  // Calculate the need matrix
  int need[MAX_PROCESSES][MAX_RESOURCES];
  for (int i = 0; i < num_processes; i++) {
    for (int j = 0; j < num_resources; j++) {
      need[i][j] = request[i][j] - allocation[i][j];
    }
  }

  // Check if the system is in a safe state
  if (is_safe(num_processes, num_resources, resource_instances, allocation,
              need)) {
    printf("No deadlock.\n");
  } else {
    printf("Deadlock detected.\n");
  }

  return 0;
}
