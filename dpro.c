//dummy program to create a number of child process and consume CPU time for testing system calls and scheduling
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
    int pid;
    int k, n; 
    int x, z;
    int first_child_pid = 0;

    if(argc < 2)
        n = 1; //Default
    else
        n = atoi(argv[1]);
    if (n < 0 || n > 20)
        n=2; 
    x = 0;
    pid = 0;

    for ( k = 0; k < n; k++ ) {
        pid = fork();
        if (pid < 0) {
            printf(1, "%d failed in fork!\n", getpid());
        } else if (pid > 0) {
            // parent process
            printf(1, "Parent %d created child %d\n", getpid(), pid);
            // Save the first child's pid for priority change
            if (k == 0) {
                first_child_pid = pid;
            }
        }
        else {
            // child process
            printf(1, "Child %d created\n", getpid());
            for (z = 0; z < 800000000; z+=1) {
                x = x + 3.14*89.64;     // Useless calculation to consume CPU Time
                if (z % 100000000 == 0) {
                    printf(1, "Child %d still running...\n", getpid());
                }
            }
            printf(1, "Child %d exiting\n", getpid());
            exit();
        }
    }

    // Give children time to start, then show process list
    sleep(10);
    printf(1, "\n=== Process List (Before Priority Change) ===\n");
    cps();
    printf(1, "===================\n\n");

    // Change priority of first child process
    if (first_child_pid > 0) {
        printf(1, "Changing priority of process %d to 5\n", first_child_pid);
        chpr(first_child_pid, 5);
        sleep(5);
        printf(1, "\n=== Process List (After Priority Change) ===\n");
        cps();
        printf(1, "===================\n\n");
    }

    for (k = 0; k < n; k++) {
        wait();
    }
    exit();
}