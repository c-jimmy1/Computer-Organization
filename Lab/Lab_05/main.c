#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_HISTORY_SIZE 50

typedef struct {
    char *commands[MAX_HISTORY_SIZE];
    int count;
} History;

void print_time(){
    time_t current_time;
    struct tm *time_info;

    time    (&current_time);

    time_info = localtime(&current_time);

    printf("Current time: %02d:%02d:%02d\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

void print_date() {
    time_t t = time(NULL);
    printf("Current date: %s", ctime(&t));
}

void print_history(History *history) {
    printf("Command history:\n");
    for (int i = 0; i < history->count; i++) {
        printf("%d: %s\n", i + 1, history->commands[i]);
    }
}

void free_history(History *history) {
    for (int i = 0; i < history->count; i++) {
        free(history->commands[i]);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    History history;
    history.count = 0;

    while (1) {
        printf("MyShell> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        char *newline = strchr(command, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }

        if (history.count < MAX_HISTORY_SIZE) {
            history.commands[history.count] = strdup(command);
            history.count++;
        } else {
            free(history.commands[0]);  // Free the oldest entry
            for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
                history.commands[i] = history.commands[i + 1];  // Shift history
            }
            history.commands[MAX_HISTORY_SIZE - 1] = strdup(command);
        }

        if (strcmp(command, "exit") == 0) {
            free_history(&history);
            printf("Exiting the shell. Goodbye!\n");
            break;
        }

        if (strcmp(command, "time") == 0) {
            print_time();
        } else if (strcmp(command, "date") == 0) {
            print_date();
        } else if (strcmp(command, "history") == 0) {
            print_history(&history);
        } else if (strcmp(command, "echo") == 0) {
            char *arg = strtok(NULL, " ");
            while (arg != NULL) {
                printf("%s ", arg);
                arg = strtok(NULL, " ");
            }
            printf("\n");
        } else {
            int ret_val = system(command);
            if (ret_val == -1) {
                printf("Failed to execute the command.\n");
            }
        }
    }

    return 0;
}