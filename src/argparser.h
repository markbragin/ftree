#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include "dynamic_array.h"

typedef struct {
    unsigned options;
    DynamicArray dirnames;
} UserInput;

/* Returns struct with dynamic array dirnames. Caller must destroy id
 * via da_destroy. */
UserInput parse_args(int argc, char **argv);

#endif
