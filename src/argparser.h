#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include "dynamic_array.h"
#include "tree.h"

/* Returns dynamic array of dirnames and sets options for the given pointer.
 * Caller must destroy it via da_destroy. */
DynamicArray *parse_args(int argc, char **argv, Options *opts);

/* Prints usage to stdout */
void usage(void);

#endif
