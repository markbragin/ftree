/*
 * Dynamic array of char* which allocates memory for each element and
 * frees it on destroy.
 */

#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

#include <stddef.h>

typedef struct DynamicArray DynamicArray;

/* Creates dynamic array on the heap. Caller must destroy it via da_destroy.
 * Capacity can be set as 0 to use default value */
DynamicArray *da_create(size_t capacity);

/* Destroys array */
void da_destroy(DynamicArray *da);

/* Pushed char* into array. Creates copy on a heap */
void da_push(DynamicArray *da, const char *value);

/* Returns size of array */
size_t da_size(DynamicArray *da);

/* Returns value at given index */
char *da_at(DynamicArray *da, size_t i);

/* Sorts values in ascending order */
void da_sort(DynamicArray *da);

#endif
