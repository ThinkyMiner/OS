#include <stdbool.h>
#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

typedef struct {
  int name;
  int allocated[MAX_RESOURCES];
  int requested[MAX_RESOURCES];
} Process;

typedef struct {
  int name;
} Resource;

bool isSafe(Process processes[], int n, Resource resources[], int m,
            int excluded[]) {
  int work[MAX_RESOURCES];
  for (int i = 0; i < m; i++) {
    work[i] = resources[i].name; // Initial number of available resources
  }

  bool finish[MAX_PROCESSES] = {false};

  for (int i = 0; i < n; i++) {
    if (excluded[i] == 1 || finish[i] == true) {
      continue; // Skip excluded or finished processes
    }
    bool canFinish = true;
    for (int j = 0; j < m; j++) {
      if (processes[i].requested[j] > work[j]) {
        canFinish = false;
        break;
      }
    }
    if (canFinish) {
      for (int j = 0; j < m; j++) {
        work[j] += processes[i].allocated[j];
      }
      finish[i] = true;
    }
  }

  // Check if all processes are finished
  for (int i = 0; i < n; i++) {
    if (excluded[i] == 0 && finish[i] == false) {
      return false;
    }
  }

  return true;
}

void findSmallestDeadlockFreeSet(Process processes[], int n,
                                 Resource resources[], int m,
                                 int currentExcluded[], int size, int *minSize,
                                 int *minSet) {
  if (size >= n) {
    return; // Reached end of recursion
  }

  // Check if current excluded set avoids deadlock
  if (isSafe(processes, n, resources, m, currentExcluded)) {
    if (size < *minSize) {
      *minSize = size;
      for (int i = 0; i < n; i++) {
        minSet[i] = currentExcluded[i];
      }
    }
    return; // No need to explore further if smaller set found
  }

  // Try excluding each remaining process
  for (int i = 0; i < n; i++) {
    if (currentExcluded[i] == 0) {
      currentExcluded[i] = 1;
      findSmallestDeadlockFreeSet(processes, n, resources, m, currentExcluded,
                                  size + 1, minSize, minSet);
      currentExcluded[i] = 0; // Backtrack
    }
  }
}

int main() {
  int n, m; // Number of processes and resources

  printf("Enter the number of processes: ");
  scanf("%d", &n);

  printf("Enter the number of resources: ");
  scanf("%d", &m);

  Process processes[MAX_PROCESSES];
  Resource resources[MAX_RESOURCES];

  printf("Enter details of processes:\n");
  for (int i = 0; i < n; i++) {
    processes[i].name = i;
    printf("Process %d:\n", i);
    printf("Allocated resources (space-separated): ");
    for (int j = 0; j < m; j++) {
      scanf("%d", &processes[i].allocated[j]);
    }
    printf("Requested resources (space-separated): ");
    for (int j = 0; j < m; j++) {
      scanf("%d", &processes[i].requested[j]);
    }
  }

  printf("Enter details of resources:\n");
  for (int i = 0; i < m; i++) {
    resources[i].name = i;
    printf("Resource %d: ", i);
    scanf("%d", &resources[i].name);
  }

  int minSize = n;
  int minSet[MAX_PROCESSES] = {
      0}; // To store process indices in the smallest set
  int excluded[MAX_PROCESSES] = {0}; // Initialize excluded array with zeros

  findSmallestDeadlockFreeSet(processes, n, resources, m, excluded, 0, &minSize,
                              minSet);

  printf("Smallest deadlock-free set of processes: ");
  for (int i = 0; i < n; i++) {
    if (minSet[i] == 1) {
      printf("%d ", i);
    }
  }
  printf("\n");

  return 0;
}
