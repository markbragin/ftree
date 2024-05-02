#ifndef _TREE_H_
#define _TREE_H_

#include "bool.h"
#define MAX_PATH_LEN 8192

/* Struct for holding given options */
typedef struct {
    bool nocolor;
    bool size;
    bool human;
    bool all;
    bool dironly;
    bool ascii;
    int depth;
} Options;

/* Global instance of Options */
extern Options OPTIONS;

/* /1* Options *1/ */
/* typedef enum { */
/*     T_NOCOL   = (1 << 0), */
/*     T_SIZE    = (1 << 1), */
/*     T_HUMAN   = (1 << 2), */
/*     T_ALL     = (1 << 3), */
/*     T_DIRONLY = (1 << 4), */
/*     T_ASCII   = (1 << 5) */
/* } Options; */

/* Prints files in given dir recursively */
void print_tree(const char *dirname);

#endif
