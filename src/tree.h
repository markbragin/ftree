#ifndef _TREE_H_
#define _TREE_H_

#define MAX_PATH_LEN 8192

/* Options */
typedef enum {
    T_NOCOL   = (1 << 0),
    T_SIZE    = (1 << 1),
    T_HUMAN   = (1 << 2),
    T_ALL     = (1 << 3),
    T_DIRONLY = (1 << 4)
} Options;

/* Prints files in given dir recursively */
void print_tree(const char *dirname, unsigned options);

#endif
