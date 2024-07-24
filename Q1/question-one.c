#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#define INTERVAL 2

double get_cpu_usage() {
    FILE* file = fopen("/proc/loadavg", "r");
    double loadavg;
    if (file == NULL) {
        perror("Error opening /proc/loadavg");
        return -1;
    }
    fscanf(file, "%lf", &loadavg);
    fclose(file);
    return loadavg * 100;  // Convert to percentage
}

double get_memory_usage() {
    FILE* file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/meminfo");
        return -1;
    }

    long total_mem = 0, free_mem = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "MemTotal: %ld kB", &total_mem) == 1) continue;
        if (sscanf(line, "MemFree: %ld kB", &free_mem) == 1) break;
    }

    fclose(file);

    if (total_mem == 0) return -1;
    return 100.0 * (total_mem - free_mem) / total_mem;
}

void monitor_performance() {
    FILE* output_file = fopen("performance_log.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    for (int i = 0; i < 30; i++) {  // Run for 30 iterations (1 minute)
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timestamp[9];
        strftime(timestamp, sizeof(timestamp), "%H:%M:%S", t);

        double cpu_usage = get_cpu_usage();
        double mem_usage = get_memory_usage();

        fprintf(output_file, "%s CPU: %.2f%% MEM: %.2f%%\n", timestamp, cpu_usage, mem_usage);
        fflush(output_file);

        sleep(INTERVAL);
    }

    fclose(output_file);
}

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        char command[256];
        snprintf(command, sizeof(command), 
                 "strace -e trace=open,close,read,write -o syscalls.txt -p %d", 
                 getppid());
        system(command);
        exit(0);
    } else if (pid > 0) {
        // Parent process
        sleep(1);  // Give child process time to start strace
        monitor_performance();
        kill(pid, SIGTERM);  // Terminate child process
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}
