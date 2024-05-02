#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dynamic_array.h"

static unsigned MIN_CAPACITY = 8;

static void resize(DynamicArray *da);
static int cmp(const void *lhs, const void *rhs);

DynamicArray da_create(unsigned capacity)
{
    unsigned nbytes;
    DynamicArray da;

    da.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(char *) * da.capacity;
    da.size = 0;
    da.items = malloc(nbytes);

    if (da.items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
    return da;
}

void da_destroy(DynamicArray *da)
{
    unsigned i;

    for (i = 0; i < da->size; i++)
        free(da->items[i]);
    free(da->items);
}

void da_push(DynamicArray *da, const char *value)
{
    if (da->size == da->capacity)
        resize(da);

    da->items[da->size] = malloc(strlen(value) + 1);
    if (da->items[da->size] == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
    strcpy(da->items[da->size], value);
    da->size++;
}

/* Doubles the array size */
static void resize(DynamicArray *da)
{
    unsigned new_capacity;
    int i;

    i = 1;
    do {
        new_capacity = da->capacity * (1 + 1. / i);
        da->items = realloc(da->items, new_capacity * sizeof(char *));
        i++;
    } while (da->items == NULL && i <= 4);

    da->capacity = new_capacity;
    if (da->items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
}

char *da_back(DynamicArray *da)
{
    return da->items[da->size - 1];
}

void da_pop(DynamicArray *da)
{
    if (da->size > 0)
        da->size--;
}

int da_contains(DynamicArray *da, const char *value)
{
    unsigned i;

    for (i = 0; i < da->size; i++)
        if (strcmp(da->items[i], value) == 0)
            return 1;
    return 0;
}

void da_sort(DynamicArray *da)
{
    qsort(da->items, da->size, sizeof(char *), cmp);
}

static int cmp(const void *lhs, const void *rhs)
{
    return strcasecmp((*(const char **)lhs), (*(const char **)rhs));
}
