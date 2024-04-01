#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define MAX_RESOURCES 100

bool is_safe(int num_processes, int num_resources, int *available, int allocation[MAX_PROCESSES][MAX_RESOURCES], int need[MAX_PROCESSES][MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int sequence[MAX_PROCESSES];
    int count = 0;

    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }

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
        if (!found) {
            break;
        }
    }

    for (int i = 0; i < num_processes; i++) {
        if (!finish[i]) {
            return false;
        }
    }
    return true;
}

void find_deadlock_processes(int num_processes, int num_resources, int *resource_instances, int allocation[MAX_PROCESSES][MAX_RESOURCES], int request[MAX_PROCESSES][MAX_RESOURCES], int *deadlock_processes, int *num_deadlock_processes) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];

    for (int i = 0; i < num_resources; i++) {
        available[i] = resource_instances[i];
    }

    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            need[i][j] = request[i][j] - allocation[i][j];
            available[j] -= allocation[i][j];
        }
    }

    for (int num_terminate = 1; num_terminate <= num_processes; num_terminate++) {
        int terminate_processes[MAX_PROCESSES];
        for (int i = 0; i < num_processes; i++) {
            int bit = 1;
            for (int j = 0; j < num_terminate; j++) {
                if ((i & bit) != 0) {
                    terminate_processes[j] = i;
                }
                bit <<= 1;
            }

            int new_available[MAX_RESOURCES];
            int new_allocation[MAX_PROCESSES][MAX_RESOURCES];
            int new_need[MAX_PROCESSES][MAX_RESOURCES];

            for (int j = 0; j < num_resources; j++) {
                new_available[j] = available[j];
            }

            for (int j = 0; j < num_processes; j++) {
                bool terminate = false;
                for (int k = 0; k < num_terminate; k++) {
                    if (terminate_processes[k] == j) {
                        terminate = true;
                        break;
                    }
                }
                if (terminate) {
                    for (int k = 0; k < num_resources; k++) {
                        new_available[k] += allocation[j][k];
                        new_allocation[j][k] = 0;
                        new_need[j][k] = 0;
                    }
                } else {
                    for (int k = 0; k < num_resources; k++) {
                        new_allocation[j][k] = allocation[j][k];
                        new_need[j][k] = need[j][k];
                    }
                }
            }

            if (is_safe(num_processes, num_resources, new_available, new_allocation, new_need)) {
                *num_deadlock_processes = num_terminate;
                for (int j = 0; j < num_terminate; j++) {
                    deadlock_processes[j] = terminate_processes[j];
                }
                return;
            }
        }
    }

    *num_deadlock_processes = 0;
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

    int deadlock_processes[MAX_PROCESSES];
    int num_deadlock_processes;

    find_deadlock_processes(num_processes, num_resources, resource_instances, allocation, request, deadlock_processes, &num_deadlock_processes);

    if (num_deadlock_processes == 0) {
        printf("No deadlock.\n");
    } else {
        printf("Deadlock exists.\n");
        printf("The smallest set of processes whose termination ends the deadlock: ");
        for (int i = 0; i < num_deadlock_processes; i++) {
            printf("%d ", deadlock_processes[i]);
        }
        printf("\n");
    }

    return 0;
}