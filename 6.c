// Unions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

typedef struct {
    UnionData data;
    ValueType type;
} TaggedData;

int set_int(TaggedData *r, int value) {
    if (r == NULL) {
        return 0;
    }

    r->data.i = value;
    r->type = VALUE_INT;
    return 1;
}

int set_float(TaggedData *r, float value) {
    if (r == NULL) {
        return 0;
    }

    r->data.f = value;
    r->type = VALUE_FLOAT;
    return 1;
}

int set_text(TaggedData *r, const char *value) {
    if (r == NULL || value == NULL || strlen(value) >= 20) {
        return 0;
    }

    strcpy(r->data.str, value);
    r->type = VALUE_TEXT;
    return 1;
}

void display_record(const TaggedData *r) {
    if (r == NULL) {
        printf("Invalid record.\n");
        return;
    }

    switch (r->type) {
        case VALUE_INT:
            printf("Integer: %d\n", r->data.i);
            break;

        case VALUE_FLOAT:
            printf("Float: %.2f\n", r->data.f);
            break;

        case VALUE_TEXT:
            printf("Text: %s\n", r->data.str);
            break;

        default:
            printf("Invalid type.\n");
    }
}

int main(void) {
    UnionData udata;
    StructData sdata;

    TaggedData records[5];

    // sdata = (StructData) {10, 220.5, "Struct Programming"};

    sdata.i = 10;
    sdata.f = 220.5f;
    strcpy(sdata.str, "Struct Programming");

    udata.i = 10;
    printf("Union Integer: %d\n", udata.i);

    strcpy(udata.str, "Union Programming");
    printf("Union Text: %s\n", udata.str);

    udata.f = 220.5f;
    printf("Union Float: %.2f\n", udata.f);

    // Now see the values after overwriting

    printf("\nAfter overwriting:\n");
    printf("Union Float: %.2f\n", udata.f);

    // Memory Addresses - Union

    printf("\nMemory Addresses - Union\n");
    printf("Integer: %p\n", (void *)&udata.i);
    printf("Float: %p\n", (void *)&udata.f);
    printf("Text: %p\n", (void *)udata.str);

    // Memory Addresses - Struct

    printf("\nMemory Addresses - Struct\n");
    printf("Integer: %p\n", (void *)&sdata.i);
    printf("Float: %p\n", (void *)&sdata.f);
    printf("Text: %p\n", (void *)sdata.str);

    printf("\nStructure Field Offsets\n");
    printf("Integer: %zu bytes\n", offsetof(StructData, i));
    printf("Float: %zu bytes\n", offsetof(StructData, f));
    printf("Text: %zu bytes\n", offsetof(StructData, str));

    printf("\nSize and Alignment\n");
    printf("Union size: %zu bytes\n", sizeof(UnionData));
    printf("Structure size: %zu bytes\n", sizeof(StructData));
    printf("Union alignment: %zu bytes\n", _Alignof(UnionData));
    printf("Structure alignment: %zu bytes\n", _Alignof(StructData));

    printf("\nMemory for 100 records\n");
    printf("100 unions: %zu bytes\n", 100 * sizeof(UnionData));
    printf("100 structures: %zu bytes\n", 100 * sizeof(StructData));

    set_int(&records[0], 10);
    set_float(&records[1], 220.5f);
    set_text(&records[2], "Union Programming");
    set_int(&records[3], 50);
    set_text(&records[4], "Hello");

    printf("\nTagged Records\n");

    for (int i = 0; i < 5; i++) {
        printf("Record %d: ", i + 1);
        display_record(&records[i]);
    }

    printf("\nChanging Record 1 type\n");

    set_text(&records[0], "Hello");
    display_record(&records[0]);

    set_float(&records[0], 3.14f);
    display_record(&records[0]);

    printf("\nTagged Record Size: %zu bytes\n",
           sizeof(TaggedData));

    printf("Plain Union Size: %zu bytes\n", sizeof(UnionData));
    printf("Plain Structure Size: %zu bytes\n", sizeof(StructData));

    printf("\nObservation:\n");
    printf("Union members share the same memory location.\n");
    printf("Structure members have separate memory locations.\n");
    printf("A union saves memory when only one value is needed at a time.\n");
    printf("Structure padding and alignment can increase its size.\n");

    return 0;
}

