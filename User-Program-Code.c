#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define FILENAME "pids.txt"

void write_pid_info() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d %d\n", getpid(), getppid());
    fclose(file);
}

void allocate_memory() {
    char *buffer = malloc(1024 * 1024); // Allocate 1MB
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, 1024 * 1024); // Prevent optimization
}

void create_children(int level) {
    if (level <= 0) return;

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child
            write_pid_info();
            allocate_memory();
            create_children(level - 1);
            sleep(10); // Keep process alive
            exit(EXIT_SUCCESS);
        }
    }
}

struct Process {
    pid_t pid;
    pid_t ppid;
    int visited;
};

void print_tree(struct Process *processes, int count, pid_t current_pid, int depth) {
    for (int i = 0; i < count; i++) {
        if (processes[i].ppid == current_pid && !processes[i].visited) {
            processes[i].visited = 1;
            for (int j = 0; j < depth; j++) printf("│   ");
            printf("├─ %d\n", processes[i].pid);

            char path[256];
            snprintf(path, sizeof(path), "/proc/%d/maps", processes[i].pid);
            FILE *file = fopen(path, "r");
            if (file) {
                char line[1024];
                while (fgets(line, sizeof(line), file)) {
                    for (int j = 0; j < depth + 1; j++) printf("│   ");
                    printf("│   %s", line);
                }
                fclose(file);
            }
            print_tree(processes, count, processes[i].pid, depth + 1);
        }
    }
}

int main() {
    // Initialize PID file
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(file);
    write_pid_info(); // Write main process info

    create_children(2); // Create 2 levels of children
    sleep(2); // Allow time for all processes to write

    // Read PID info
    file = fopen(FILENAME, "r");
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    struct Process processes[100];
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) && count < 100) {
        sscanf(line, "%d %d", &processes[count].pid, &processes[count].ppid);
        processes[count].visited = 0;
        count++;
    }
    fclose(file);

    // Find main PID (root)
    pid_t main_pid = getpid();
    printf("%d\n", main_pid);

    // Print main process maps
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/maps", main_pid);
    FILE *maps_file = fopen(path, "r");
    if (maps_file) {
        char line[1024];
        while (fgets(line, sizeof(line), maps_file)) {
            printf("│   %s", line);
        }
        fclose(maps_file);
    }

    print_tree(processes, count, main_pid, 0);

    // Cleanup: Terminate all child processes
    for (int i = 0; i < count; i++) {
        if (processes[i].pid != main_pid) {
            kill(processes[i].pid, SIGTERM);
        }
    }

    return EXIT_SUCCESS;
}

