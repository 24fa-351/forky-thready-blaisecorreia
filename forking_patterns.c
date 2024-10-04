#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void process_behavior(int process_number) {
    printf("Process %d (PID %d) beginning.\n", process_number, getpid());
    int sleep_time = rand() % 8 + 1; 
    sleep(sleep_time);
    printf("Process %d (PID %d) exiting.\n", process_number, getpid());
}
void pattern1(int n) {
    for (int i = 1; i <= n; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            process_behavior(i);
            exit(0);  
        }
    }

    for (int i = 0; i < n; ++i) {
        wait(NULL);
    }
}


void pattern2(int n) {
    for (int i = 1; i <= n; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            process_behavior(i);
            exit(0); 
        } else {
            wait(NULL);
        }
    }
}

void pattern3(int n) {
    if (n < 1) return;
    
    pid_t left_child = fork();
    if (left_child == 0) {
        if (n > 1) pattern3(n / 2);  
        process_behavior(2 * n - 1);
        exit(0);
    }

    pid_t right_child = fork();
    if (right_child == 0) {
        if (n > 2) pattern3(n / 2);
        process_behavior(2 * n);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_of_processes> <pattern_number>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    srand(time(NULL));


    if (num_processes < 1 || num_processes > 256) {
        printf("Error: Number of processes must be between 1 and 256.\n");
        return 1;
    }

    switch (pattern) {
        case 1:
            printf("Using Pattern 1: Fork all at once\n");
            pattern1(num_processes);
            break;
        case 2:
            printf("Using Pattern 2: Sequential process creation\n");
            pattern2(num_processes);
            break;
        case 3:
            printf("Using Pattern 3: Tree structure process creation\n");
            pattern3(num_processes);
            break;
        default:
            printf("Error: Invalid pattern number. Choose 1, 2, or 3.\n");
            return 1;
    }

    return 0;
}