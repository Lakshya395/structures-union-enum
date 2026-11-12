#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum to track the type of data stored

typedef enum {
    TYPE_EMPTY, INT, FLOAT, CHAR, STRING
} DataType;

// Union to store different types of data

typedef union {
    int int_value;
    float float_value;
    char char_value;
    char *string_value;
} Data;

// Struct to encapsulate the data and its type

typedef struct {
    Data data;
    DataType type;
} Container;

// Function declarations

int container_init(Container *c);
int container_clear(Container *c);

int set_int(Container *c, int value);
int set_float(Container *c, float value);
int set_char(Container *c, char value);
int set_string(Container *c, const char *value);

int get_int(const Container *c, int *out);
int get_float(const Container *c, float *out);
int get_char(const Container *c, char *out);
int get_string(const Container *c, const char **out);

int container_copy(Container *dest, const Container *src);
int container_print(const Container *c);
const char *container_type_name(const Container *c);

int main(void) {
    Container items[6], copy;
    int number;
    float decimal;
    char character;
    const char *text;

    for (int i = 0; i < 6; i++) {
        container_init(&items[i]);
    }

    container_init(&copy);

    if (!set_int(&items[0], 10) ||
        !set_float(&items[1], 3.14f) ||
        !set_char(&items[2], 'A') ||
        !set_string(&items[3], "Hello") ||
        !set_int(&items[4], 0) ||
        !set_string(&items[5], "Data")) {
        goto cleanup;
    }

    printf("Container Records\n");

    for (int i = 0; i < 6; i++) {
        printf("%d. ", i);
        container_print(&items[i]);
    }

    printf("\nGetter Values\n");

    if (get_int(&items[0], &number)) {
        printf("Integer: %d\n", number);
    }

    if (get_float(&items[1], &decimal)) {
        printf("Float: %.2f\n", decimal);
    }

    if (get_char(&items[2], &character)) {
        printf("Character: %c\n", character);
    }

    if (get_string(&items[3], &text)) {
        printf("String: %s\n", text);
    }

    printf("Wrong getter: %d\n",
           get_int(&items[2], &number));

    container_copy(&copy, &items[5]);
    set_string(&items[5], "Changed");

    printf("\nOriginal: ");
    container_print(&items[5]);

    printf("Copied: ");
    container_print(&copy);

    container_copy(&copy, &copy);

    set_string(&items[5], items[5].data.string_value);

    set_string(&items[3], "Hello");
    set_int(&items[3], 20);
    set_string(&items[3], "World");
    set_float(&items[3], 5.5f);

    printf("\nAfter Type Changes: ");
    container_print(&items[3]);

    set_string(&items[3], "");
    container_print(&items[3]);

    container_clear(&items[3]);
    container_clear(&items[3]);

    printf("After Clear: ");
    container_print(&items[3]);

    // Free any allocated memory

cleanup:
    for (int i = 0; i < 6; i++) {
        container_clear(&items[i]);
    }

    container_clear(&copy);

    return 0;
}

// Function definitions

int container_init(Container *c) {
    if (c == NULL) {
        return 0;
    }

    c->type = TYPE_EMPTY;
    c->data.int_value = 0;

    return 1;
}

int container_clear(Container *c) {
    if (c == NULL) {
        return 0;
    }

    if (c->type == STRING) {
        free(c->data.string_value);
    }

    c->type = TYPE_EMPTY;
    c->data.int_value = 0;

    return 1;
}

int set_int(Container *c, int value) {
    if (c == NULL || !container_clear(c)) {
        return 0;
    }

    c->data.int_value = value;
    c->type = INT;

    return 1;
}

int set_float(Container *c, float value) {
    if (c == NULL || !container_clear(c)) {
        return 0;
    }

    c->data.float_value = value;
    c->type = FLOAT;

    return 1;
}

int set_char(Container *c, char value) {
    if (c == NULL || !container_clear(c)) {
        return 0;
    }

    c->data.char_value = value;
    c->type = CHAR;

    return 1;
}

int set_string(Container *c, const char *value) {
    if (c == NULL || value == NULL) {
        return 0;
    }

    size_t len = strlen(value);
    char *temp = malloc(len + 1);

    if (temp == NULL) {
        return 0;
    }

    memcpy(temp, value, len + 1);

    // Free previous string if any
    container_clear(c);

    c->data.string_value = temp;
    c->type = STRING;

    return 1;
}

int get_int(const Container *c, int *out) {
    if (c == NULL || out == NULL || c->type != INT) {
        return 0;
    }

    *out = c->data.int_value;

    return 1;
}

int get_float(const Container *c, float *out) {
    if (c == NULL || out == NULL || c->type != FLOAT) {
        return 0;
    }

    *out = c->data.float_value;

    return 1;
}

int get_char(const Container *c, char *out) {
    if (c == NULL || out == NULL || c->type != CHAR) {
        return 0;
    }

    *out = c->data.char_value;

    return 1;
}

int get_string(const Container *c, const char **out) {
    if (c == NULL || out == NULL ||
        c->type != STRING || c->data.string_value == NULL) {
        return 0;
    }

    *out = c->data.string_value;

    return 1;
}

int container_copy(Container *dest, const Container *src) {
    if (dest == NULL || src == NULL) {
        return 0;
    }

    if (dest == src) {
        return 1;
    }

    if (src->type == TYPE_EMPTY) {
        return container_clear(dest);
    }

    if (src->type == STRING) {
        return set_string(dest, src->data.string_value);
    }

    if (src->type != INT &&
        src->type != FLOAT &&
        src->type != CHAR) {
        return 0;
    }

    Data temp = src->data;
    DataType type = src->type;

    container_clear(dest);

    dest->data = temp;
    dest->type = type;

    return 1;
}

const char *container_type_name(const Container *c) {
    if (c == NULL) {
        return "Invalid";
    }

    switch (c->type) {
        case TYPE_EMPTY: return "Empty";
        case INT: return "Integer";
        case FLOAT: return "Float";
        case CHAR: return "Character";
        case STRING: return "String";
        default: return "Invalid";
    }
}

int container_print(const Container *c) {
    if (c == NULL) {
        return 0;
    }

    printf("%s: ", container_type_name(c));

    switch (c->type) {
        case TYPE_EMPTY:
            printf("No value\n");
            break;

        case INT:
            printf("%d\n", c->data.int_value);
            break;

        case FLOAT:
            printf("%.2f\n", c->data.float_value);
            break;

        case CHAR:
            printf("%c\n", c->data.char_value);
            break;

        case STRING:
            if (c->data.string_value == NULL) {
                return 0;
            }

            printf("%s\n", c->data.string_value);
            break;

        default:
            return 0;
    }

    return 1;
}
