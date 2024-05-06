#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dynamic_array.h"

struct DynamicArray {
    char **items;
    size_t size;
    size_t capacity;
};

static size_t MIN_CAPACITY = 32;

static void resize(DynamicArray *da);
static int cmp(const void *lhs, const void *rhs);

DynamicArray *da_create(size_t capacity)
{
    size_t nbytes;
    DynamicArray *da;

    da = malloc(sizeof(DynamicArray));
    if (!da) {
        perror("Can't allocate memory");
        abort();
    }

    da->capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes       = sizeof(char *) * da->capacity;
    da->size     = 0;
    da->items    = malloc(nbytes);

    if (da->items == NULL) {
        perror("Can't allocate memory");
        abort();
    }

    return da;
}

void da_destroy(DynamicArray *da)
{
    size_t i;

    for (i = 0; i < da->size; i++)
        free(da->items[i]);
    free(da->items);
    free(da);
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
    size_t new_capacity;
    int i;

    i = 1;
    do {
        new_capacity = da->capacity * (1 + 1. / i);
        da->items    = realloc(da->items, new_capacity * sizeof(char *));
        i++;
    } while (da->items == NULL && i <= 4);

    da->capacity = new_capacity;
    if (da->items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
}

size_t da_size(DynamicArray *da)
{
    return da->size;
}

char *da_at(DynamicArray *da, size_t i)
{
    return da->items[i];
}

void da_sort(DynamicArray *da)
{
    qsort(da->items, da->size, sizeof(char *), cmp);
}

static int cmp(const void *lhs, const void *rhs)
{
    return strcasecmp((*(const char **)lhs), (*(const char **)rhs));
}
