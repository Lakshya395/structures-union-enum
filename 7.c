#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

// Enum for command types
typedef enum {
    CMD_ADD, CMD_DELETE, CMD_VIEW, CMD_UPDATE,
    CMD_COMPLETE, CMD_FIND, CMD_HELP, CMD_EXIT, CMD_UNKNOWN
} CommandType;

typedef enum {
    PENDING, DONE
} TaskStatus;

typedef struct {
    int id;
    char description[100];
    TaskStatus status;
} Task;

// Function to convert string to CommandType
CommandType get_command_type(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }

    if (!strcmp(s, "ADD")) return CMD_ADD;
    if (!strcmp(s, "DELETE")) return CMD_DELETE;
    if (!strcmp(s, "VIEW")) return CMD_VIEW;
    if (!strcmp(s, "UPDATE")) return CMD_UPDATE;
    if (!strcmp(s, "COMPLETE")) return CMD_COMPLETE;
    if (!strcmp(s, "FIND")) return CMD_FIND;
    if (!strcmp(s, "HELP")) return CMD_HELP;
    if (!strcmp(s, "EXIT")) return CMD_EXIT;

    return CMD_UNKNOWN;
}

int read_line(const char *prompt, char *s, size_t size) {
    printf("%s", prompt);

    if (!fgets(s, size, stdin)) return 0;

    char *p = strchr(s, '\n');

    if (p) {
        *p = '\0';
    } else {
        int ch = getchar();

        if (ch != EOF && ch != '\n') {
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Input too long.\n");
            return -1;
        }
    }

    return 1;
}

int read_id(int *id) {
    char s[50], *end;
    int result = read_line("Enter task ID: ", s, sizeof(s));

    if (result != 1) return result;

    errno = 0;
    long value = strtol(s, &end, 10);

    if (errno || end == s || *end != '\0' ||
        value <= 0 || value > INT_MAX) {
        printf("Invalid ID.\n");
        return -1;
    }

    *id = (int)value;
    return 1;
}

int find_task(Task tasks[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (tasks[i].id == id) return i;
    }
    return -1;
}

int add_task(Task **tasks, int *count, size_t *capacity) {
    Task t;

    int r = read_id(&t.id);
    if (r != 1) return r == 0 ? 0 : 1;

    if (find_task(*tasks, *count, t.id) != -1) {
        printf("Duplicate ID.\n");
        return 1;
    }

    r = read_line("Description: ", t.description,
                  sizeof(t.description));

    if (r == 0) return 0;
    if (r == -1) return 1;

    if (t.description[0] == '\0') {
        printf("Description cannot be empty.\n");
        return 1;
    }

    if ((size_t)*count == *capacity) {
        size_t new_capacity = *capacity ? *capacity * 2 : 4;

        if (new_capacity > SIZE_MAX / sizeof(Task)) {
            printf("Capacity limit reached.\n");
            return 1;
        }

        Task *temp = realloc(*tasks, new_capacity * sizeof(Task));

        if (!temp) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        *tasks = temp;
        *capacity = new_capacity;
    }

    t.status = PENDING;
    (*tasks)[(*count)++] = t;

    printf("Task %d added.\n", t.id);
    return 1;
}

int delete_task(Task tasks[], int *count) {
    int id, r = read_id(&id);
    if (r != 1) return r == 0 ? 0 : 1;

    int i = find_task(tasks, *count, id);

    if (i == -1) {
        printf("Task not found.\n");
        return 1;
    }

    for (; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    (*count)--;
    printf("Task %d deleted.\n", id);
    return 1;
}

int update_task(Task tasks[], int count) {
    int id, r = read_id(&id);
    if (r != 1) return r == 0 ? 0 : 1;

    int i = find_task(tasks, count, id);

    if (i == -1) {
        printf("Task not found.\n");
        return 1;
    }

    r = read_line("New description: ",
                  tasks[i].description,
                  sizeof(tasks[i].description));

    if (r == 0) return 0;
    if (r == -1) return 1;

    if (!tasks[i].description[0]) {
        printf("Description cannot be empty.\n");
        return 1;
    }

    printf("Task %d updated.\n", id);
    return 1;
}

void view_tasks(Task tasks[], int count) {
    if (!count) {
        printf("Task list is empty.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("%d | %s | %s\n", tasks[i].id,
               tasks[i].description,
               tasks[i].status == DONE ? "DONE" : "PENDING");
    }
}

int complete_task(Task tasks[], int count) {
    int id, r = read_id(&id);
    if (r != 1) return r == 0 ? 0 : 1;

    int i = find_task(tasks, count, id);

    if (i == -1) {
        printf("Task not found.\n");
    } else {
        tasks[i].status = DONE;
        printf("Task %d completed.\n", id);
    }

    return 1;
}

void help(void) {
    printf("ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT\n");
}

// Main program
int main(void) {
    Task *tasks = NULL;
    int count = 0, running = 1;
    size_t capacity = 0;
    char input[100];

    while (running) {
        int r = read_line("\nEnter command: ", input, sizeof(input));

        if (r == 0) break;
        if (r == -1) continue;

        CommandType cmd = get_command_type(input);

        switch (cmd) {
            case CMD_ADD:
                running = add_task(&tasks, &count, &capacity);
                break;

            case CMD_DELETE:
                running = delete_task(tasks, &count);
                break;

            case CMD_VIEW:
                view_tasks(tasks, count);
                break;

            case CMD_UPDATE:
                running = update_task(tasks, count);
                break;

            case CMD_COMPLETE:
                running = complete_task(tasks, count);
                break;

            case CMD_FIND: {
                int id, r = read_id(&id);

                if (r == 0) {
                    running = 0;
                } else if (r == 1) {
                    int i = find_task(tasks, count, id);

                    if (i == -1) {
                        printf("Task not found.\n");
                    } else {
                        view_tasks(&tasks[i], 1);
                    }
                }
                break;
            }

            case CMD_HELP:
                help();
                break;

            case CMD_EXIT:
                running = 0;
                break;

            default:
                printf("Unknown command.\n");
        }
    }

    free(tasks);
    printf("Program ended.\n");

    return 0;
}
