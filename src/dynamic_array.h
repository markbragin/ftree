/*
 * Dynamic array of char* which allocates memory for each element and
 * frees it on destroy.
 */

#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

typedef struct {
    char **items;
    unsigned size;
    unsigned capacity;
} DynamicArray;

DynamicArray da_create(unsigned capacity);

void da_destroy(DynamicArray *da);

void da_push(DynamicArray *da, const char *value);

char *da_back(DynamicArray *da);

int da_contains(DynamicArray *da, const char *value);

void da_sort(DynamicArray *da);

#endif
