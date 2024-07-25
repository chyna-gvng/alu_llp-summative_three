#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Global variables
bool oven_closed = false;
bool oven_started = false;
bool plate_rotating = false;
bool lights_on = false;
int timer = 60;
int current_command = -1;
bool show_menu = true;

pthread_mutex_t oven_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t oven_cond = PTHREAD_COND_INITIALIZER;

// Function prototypes
void* oven_controller(void* arg);
void* countdown_timer(void* arg);
void display_menu();

int main() {
    pthread_t controller_thread, timer_thread;

    pthread_create(&controller_thread, NULL, oven_controller, NULL);
    pthread_create(&timer_thread, NULL, countdown_timer, NULL);

    // Main loop to accept user input
    while (1) {
        pthread_mutex_lock(&oven_mutex);
        if (show_menu) {
            display_menu();
            printf("Enter a command: ");
            scanf("%d", &current_command);

            if (current_command >= 0 && current_command <= 3) {
                pthread_cond_signal(&oven_cond);
            } else if (current_command == 4) {
                printf("Exiting program.\n");
                pthread_mutex_unlock(&oven_mutex);
                break;
            } else {
                printf("Invalid choice. Please try again.\n");
                current_command = -1;  // Reset to invalid command
            }
        }
        pthread_mutex_unlock(&oven_mutex);

        // Small delay to allow other threads to process the command
        usleep(100000);  // 100ms delay
    }

    // Clean up
    pthread_cancel(controller_thread);
    pthread_cancel(timer_thread);
    pthread_join(controller_thread, NULL);
    pthread_join(timer_thread, NULL);

    pthread_mutex_destroy(&oven_mutex);
    pthread_cond_destroy(&oven_cond);

    return 0;
}

void display_menu() {
    printf("\nMicrowave Oven Control Menu:\n");
    printf("0 - Close Oven Door\n");
    printf("1 - Open Oven Door\n");
    printf("2 - Start Oven\n");
    printf("3 - Stop Oven\n");
    printf("4 - Exit Program\n");
}

void* oven_controller(void* arg) {
    while (1) {
        pthread_mutex_lock(&oven_mutex);
        while (current_command == -1) {
            pthread_cond_wait(&oven_cond, &oven_mutex);
        }

        switch (current_command) {
            case 0: // Close oven
                if (!oven_closed) {
                    oven_closed = true;
                    printf("Oven closed.\n");
                } else {
                    printf("Oven is already closed.\n");
                }
                break;
            case 1: // Open oven
                if (oven_closed) {
                    oven_closed = false;
                    oven_started = false;
                    plate_rotating = false;
                    lights_on = false;
                    show_menu = true;
                    printf("Oven opened. Oven stopped, plate stopped, lights off.\n");
                } else {
                    printf("Oven is already open.\n");
                }
                break;
            case 2: // Start oven
                if (oven_closed && !oven_started) {
                    oven_started = true;
                    plate_rotating = true;
                    lights_on = true;
                    timer = 60;
                    show_menu = false;
                    printf("Oven started. Plate rotating, lights on, timer set to 60 seconds.\n");
                } else if (!oven_closed) {
                    printf("Cannot start: Oven is open.\n");
                } else if (oven_started) {
                    printf("Oven is already running.\n");
                }
                break;
            case 3: // Stop oven
                if (oven_started) {
                    oven_started = false;
                    plate_rotating = false;
                    show_menu = true;
                    printf("Oven stopped. Plate stopped.\n");
                } else {
                    printf("Oven is not running.\n");
                }
                break;
        }

        current_command = -1;
        pthread_mutex_unlock(&oven_mutex);
    }
    return NULL;
}

void* countdown_timer(void* arg) {
    while (1) {
        pthread_mutex_lock(&oven_mutex);
        if (oven_started && timer > 0) {
            printf("Timer: %d seconds\n", timer);
            timer--;
            if (timer == 0) {
                printf("Timer finished. Oven stopped.\n");
                oven_started = false;
                plate_rotating = false;
                lights_on = false;
                show_menu = true;
            }
        }
        pthread_mutex_unlock(&oven_mutex);
        sleep(1);
    }
    return NULL;
}
