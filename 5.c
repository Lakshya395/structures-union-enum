// Structures - 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

#define MAX_RECTANGLES 1000
#define TOLERANCE 0.00001f

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;

int read_line(char *str, int size) {
    if (fgets(str, size, stdin) == NULL) {
        return 0;
    }

    char *p = strchr(str, '\n');

    if (p != NULL) {
        *p = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        return -1;
    }

    return 1;
}

int read_int(const char *message, int *value) {
    char str[100], *end;

    printf("%s", message);

    if (read_line(str, sizeof(str)) != 1) {
        return 0;
    }

    errno = 0;
    long num = strtol(str, &end, 10);

    while (*end == ' ') {
        end++;
    }

    if (end == str || *end != '\0' ||
        errno == ERANGE || num < 1 || num > INT_MAX) {
        printf("Invalid input.\n");
        return 0;
    }

    *value = (int)num;
    return 1;
}

int read_float(const char *message, float *value, int allow_zero) {
    char str[100], *end;

    printf("%s", message);

    if (read_line(str, sizeof(str)) != 1) {
        return 0;
    }

    errno = 0;
    float num = strtof(str, &end);

    while (*end == ' ') {
        end++;
    }

    if (end == str || *end != '\0' || errno == ERANGE ||
        !isfinite(num) || num < 0 || (!allow_zero && num == 0)) {
        printf("Invalid value.\n");
        return 0;
    }

    *value = num;
    return 1;
}

void calculate_rectangle(Rectangle *r) {
    r->area = r->width * r->height;
    r->perimeter = 2 * (r->width + r->height);
}

int find_rectangle(Rectangle *r, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (r[i].id == id) {
            return i;
        }
    }

    return -1;
}

int is_square(Rectangle r) {
    float difference = fabsf(r.width - r.height);
    float limit = TOLERANCE * fmaxf(1.0f,
                         fmaxf(r.width, r.height));

    return difference <= limit;
}

void display_rectangle(Rectangle r) {
    printf("\nID: %d\n", r.id);
    printf("Width: %.2f\n", r.width);
    printf("Height: %.2f\n", r.height);
    printf("Area: %.2f\n", r.area);
    printf("Perimeter: %.2f\n", r.perimeter);
    printf("Type: %s\n", is_square(r) ? "Square" : "Non-square");
}

void display_rectangles(Rectangle *r, int n) {
    if (n == 0) {
        printf("No rectangles available.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        display_rectangle(r[i]);
    }
}

int add_rectangle(Rectangle **r, int *n, int *capacity) {
    Rectangle temp;

    if (!read_int("Enter rectangle ID: ", &temp.id)) {
        return 0;
    }

    if (find_rectangle(*r, *n, temp.id) != -1) {
        printf("Duplicate ID.\n");
        return 0;
    }

    if (!read_float("Enter width: ", &temp.width, 0) ||
        !read_float("Enter height: ", &temp.height, 0)) {
        return 0;
    }

    calculate_rectangle(&temp);

    if (!isfinite(temp.area) || !isfinite(temp.perimeter)) {
        printf("Dimensions are too large.\n");
        return 0;
    }

    if (*n == *capacity) {
        if (*capacity >= MAX_RECTANGLES) {
            printf("Maximum capacity reached.\n");
            return 0;
        }

        int new_capacity = (*capacity * 2 > MAX_RECTANGLES)
                           ? MAX_RECTANGLES : *capacity * 2;

        Rectangle *ptr = realloc(*r,
                          new_capacity * sizeof(Rectangle));

        if (ptr == NULL) {
            printf("Memory allocation failed.\n");
            return 0;
        }

        *r = ptr;
        *capacity = new_capacity;
    }

    (*r)[(*n)++] = temp;

    printf("Rectangle %d added successfully.\n", temp.id);
    return 1;
}

void search_rectangle(Rectangle *r, int n) {
    int id;

    if (!read_int("Enter rectangle ID: ", &id)) {
        return;
    }

    int index = find_rectangle(r, n, id);

    if (index == -1) {
        printf("Rectangle not found.\n");
    } else {
        display_rectangle(r[index]);
    }
}

void update_rectangle(Rectangle *r, int n) {
    int id;

    if (!read_int("Enter rectangle ID: ", &id)) {
        return;
    }

    int index = find_rectangle(r, n, id);

    if (index == -1) {
        printf("Rectangle not found.\n");
        return;
    }

    Rectangle temp = r[index];

    if (!read_float("Enter new width: ", &temp.width, 0) ||
        !read_float("Enter new height: ", &temp.height, 0)) {
        return;
    }

    calculate_rectangle(&temp);

    if (!isfinite(temp.area) || !isfinite(temp.perimeter)) {
        printf("Dimensions are too large.\n");
        return;
    }

    r[index] = temp;

    printf("Rectangle %d updated successfully.\n", id);
}

void delete_rectangle(Rectangle *r, int *n) {
    int id;

    if (!read_int("Enter rectangle ID: ", &id)) {
        return;
    }

    int index = find_rectangle(r, *n, id);

    if (index == -1) {
        printf("Rectangle not found.\n");
        return;
    }

    for (int i = index; i < *n - 1; i++) {
        r[i] = r[i + 1];
    }

    (*n)--;

    printf("Rectangle %d deleted successfully.\n", id);
}

void geometry_summary(Rectangle *r, int n) {
    if (n == 0) {
        printf("No rectangles available.\n");
        return;
    }

    int largest = 0, smallest = 0;
    float total = 0;

    for (int i = 0; i < n; i++) {
        total += r[i].area;

        if (r[i].area > r[largest].area) {
            largest = i;
        }

        if (r[i].area < r[smallest].area) {
            smallest = i;
        }
    }

    printf("Largest area: %.2f (ID: %d)\n",
           r[largest].area, r[largest].id);

    printf("Smallest area: %.2f (ID: %d)\n",
           r[smallest].area, r[smallest].id);

    printf("Total area: %.2f\n", total);
    printf("Average area: %.2f\n", total / n);
}

void sort_rectangles(Rectangle *r, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (r[j].area > r[j + 1].area ||
                (r[j].area == r[j + 1].area &&
                 r[j].id > r[j + 1].id)) {

                Rectangle temp = r[j];
                r[j] = r[j + 1];
                r[j + 1] = temp;
            }
        }
    }

    printf("Rectangles sorted by area.\n");
    display_rectangles(r, n);
}

void filter_rectangles(Rectangle *r, int n) {
    float threshold;

    if (!read_float("Enter area threshold: ", &threshold, 1)) {
        return;
    }

    int found = 0;

    for (int i = 0; i < n; i++) {
        if (r[i].area > threshold) {
            display_rectangle(r[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No rectangles found above this area.\n");
    }
}

int main() {
    int n = 0, capacity, choice;
    Rectangle *rectangles;

    if (!read_int("Enter initial number of rectangles: ", &capacity) ||
        capacity > MAX_RECTANGLES) {
        printf("Invalid capacity.\n");
        return 1;
    }

    rectangles = malloc(capacity * sizeof(Rectangle));

    if (rectangles == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    do {
        printf("\n1. Add Rectangle\n");
        printf("2. Display Rectangles\n");
        printf("3. Search Rectangle\n");
        printf("4. Update Rectangle\n");
        printf("5. Delete Rectangle\n");
        printf("6. Geometry Summary\n");
        printf("7. Sort Rectangles\n");
        printf("8. Filter by Area\n");
        printf("9. Exit\n");

        if (!read_int("Enter choice: ", &choice)) {
            continue;
        }

        switch (choice) {
            case 1:
                add_rectangle(&rectangles, &n, &capacity);
                break;

            case 2:
                display_rectangles(rectangles, n);
                break;

            case 3:
                search_rectangle(rectangles, n);
                break;

            case 4:
                update_rectangle(rectangles, n);
                break;

            case 5:
                delete_rectangle(rectangles, &n);
                break;

            case 6:
                geometry_summary(rectangles, n);
                break;

            case 7:
                sort_rectangles(rectangles, n);
                break;

            case 8:
                filter_rectangles(rectangles, n);
                break;

            case 9:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 9);

    free(rectangles);

    return 0;
}
