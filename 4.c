// Structures - 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>

#define MAX_CAPACITY 10000

typedef struct {
    int roll_no;
    char name[50];
    int age;
    float gpa;
} Student;

int read_line(char *str, int size) {
    if (fgets(str, size, stdin) == NULL)
        return 0;

    char *p = strchr(str, '\n');

    if (p != NULL) {
        *p = '\0';
    } else {
        int ch = getchar();

        if (ch != EOF) {
            while (ch != '\n' && ch != EOF)
                ch = getchar();
            return -1;
        }
    }

    return 1;
}

int read_int(const char *message, int *value) {
    char str[100], *end;

    printf("%s", message);

    if (read_line(str, sizeof(str)) != 1)
        return 0;

    errno = 0;
    long num = strtol(str, &end, 10);

    while (isspace((unsigned char)*end))
        end++;

    if (end == str || *end != '\0' || errno == ERANGE ||
        num < 1 || num > INT32_MAX) {
        printf("Invalid input.\n");
        return 0;
    }

    *value = (int)num;
    return 1;
}

int read_gpa(float *gpa) {
    char str[100], *end;

    printf("Enter GPA: ");

    if (read_line(str, sizeof(str)) != 1)
        return 0;

    errno = 0;
    float value = strtof(str, &end);

    while (isspace((unsigned char)*end))
        end++;

    if (end == str || *end != '\0' || errno == ERANGE ||
        !isfinite(value) || value < 0 || value > 4) {
        printf("Invalid GPA. Enter between 0 and 4.\n");
        return 0;
    }

    *gpa = value;
    return 1;
}

int find_student(Student *s, int n, int roll) {
    for (int i = 0; i < n; i++) {
        if (s[i].roll_no == roll)
            return i;
    }

    return -1;
}

int add_student(Student **s, int *n, int *capacity) {
    Student temp;

    if (!read_int("Enter roll number: ", &temp.roll_no))
        return 0;

    if (find_student(*s, *n, temp.roll_no) != -1) {
        printf("Roll number already exists.\n");
        return 0;
    }

    printf("Enter name: ");

    if (read_line(temp.name, sizeof(temp.name)) != 1 ||
        strlen(temp.name) == 0 || strlen(temp.name) > 49) {
        printf("Invalid name.\n");
        return 0;
    }

    if (!read_int("Enter age: ", &temp.age))
        return 0;

    if (temp.age < 16 || temp.age > 100) {
        printf("Age must be between 16 and 100.\n");
        return 0;
    }

    if (!read_gpa(&temp.gpa))
        return 0;

    if (*n == *capacity) {
        if (*capacity >= MAX_CAPACITY) {
            printf("Maximum capacity reached.\n");
            return 0;
        }

        int new_capacity = (*capacity > MAX_CAPACITY / 2)
                           ? MAX_CAPACITY : *capacity * 2;

        if ((size_t)new_capacity > SIZE_MAX / sizeof(Student))
            return 0;

        Student *temp_ptr = realloc(*s,
                            (size_t)new_capacity * sizeof(Student));

        if (temp_ptr == NULL) {
            printf("Memory allocation failed.\n");
            return 0;
        }

        *s = temp_ptr;
        *capacity = new_capacity;
    }

    (*s)[*n] = temp;
    (*n)++;

    printf("Student added successfully.\n");
    return 1;
}

void display_students(Student *s, int n) {
    if (n == 0) {
        printf("No records available.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s %-10s\n",
           "Roll No", "Name", "Age", "GPA");

    for (int i = 0; i < n; i++) {
        printf("%-10d %-20s %-10d %-10.2f\n",
               s[i].roll_no, s[i].name, s[i].age, s[i].gpa);
    }
}

void search_student(Student *s, int n) {
    int roll;

    if (!read_int("Enter roll number: ", &roll))
        return;

    int index = find_student(s, n, roll);

    if (index == -1)
        printf("Student not found.\n");
    else
        display_students(&s[index], 1);
}

void update_student(Student *s, int n) {
    int roll;

    if (!read_int("Enter roll number: ", &roll))
        return;

    int index = find_student(s, n, roll);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    Student temp = s[index];

    printf("Enter new name: ");

    if (read_line(temp.name, sizeof(temp.name)) != 1 ||
        strlen(temp.name) == 0 || strlen(temp.name) > 49) {
        printf("Invalid name.\n");
        return;
    }

    if (!read_int("Enter new age: ", &temp.age))
        return;

    if (temp.age < 16 || temp.age > 100) {
        printf("Invalid age.\n");
        return;
    }

    if (!read_gpa(&temp.gpa))
        return;

    s[index] = temp;
    printf("Record updated successfully.\n");
}

void delete_student(Student *s, int *n) {
    int roll;

    if (!read_int("Enter roll number: ", &roll))
        return;

    int index = find_student(s, *n, roll);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    for (int i = index; i < *n - 1; i++)
        s[i] = s[i + 1];

    (*n)--;
    printf("Student deleted successfully.\n");
}

void summary(Student *s, int n) {
    if (n == 0) {
        printf("No records available.\n");
        return;
    }

    int high = 0, low = 0;
    float total = 0;

    for (int i = 0; i < n; i++) {
        total += s[i].gpa;

        if (s[i].gpa > s[high].gpa)
            high = i;

        if (s[i].gpa < s[low].gpa)
            low = i;
    }

    printf("Highest GPA: %s (%.2f)\n",
           s[high].name, s[high].gpa);

    printf("Lowest GPA: %s (%.2f)\n",
           s[low].name, s[low].gpa);

    printf("Average GPA: %.2f\n", total / n);
}

void sort_students(Student *s, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (s[j].gpa < s[j + 1].gpa ||
                (s[j].gpa == s[j + 1].gpa &&
                 s[j].roll_no > s[j + 1].roll_no)) {
                Student temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }

    printf("Sorted by GPA (descending). Original array rearranged.\n");
    display_students(s, n);
}

int main() {
    Student *students;
    int n = 0, capacity, choice;

    if (!read_int("Enter initial capacity: ", &capacity) ||
        capacity > MAX_CAPACITY) {
        printf("Invalid capacity.\n");
        return 1;
    }

    if ((size_t)capacity > SIZE_MAX / sizeof(Student))
        return 1;

    students = malloc((size_t)capacity * sizeof(Student));

    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    do {
        printf("\n1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. GPA Summary\n");
        printf("7. Sort Students\n");
        printf("8. Exit\n");

        if (!read_int("Enter choice: ", &choice))
            continue;

        switch (choice) {
            case 1:
                add_student(&students, &n, &capacity);
                break;

            case 2:
                display_students(students, n);
                break;

            case 3:
                search_student(students, n);
                break;

            case 4:
                update_student(students, n);
                break;

            case 5:
                delete_student(students, &n);
                break;

            case 6:
                summary(students, n);
                break;

            case 7:
                sort_students(students, n);
                break;

            case 8:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 8);

    free(students);

    return 0;
}
