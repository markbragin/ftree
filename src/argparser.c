#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"
#include "bool.h"
#include "dynamic_array.h"
#include "tree.h"

DynamicArray parse_args(int argc, char **argv, Options *opts)
{
    int i;
    DynamicArray dirnames;

    dirnames = da_create(0);

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            opts->size = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            opts->human = true;
        } else if (strcmp(argv[i], "-nc") == 0) {
            opts->nocolor = true;
        } else if (strcmp(argv[i], "-a") == 0) {
            opts->all = true;
        } else if (strcmp(argv[i], "-d") == 0) {
            opts->dironly = true;
        } else if (strcmp(argv[i], "-A") == 0) {
            opts->ascii = true;
        } else if (strcmp(argv[i], "-L") == 0) {
            if (++i == argc || (opts->depth = atoi(argv[i])) <= 0) {
                fprintf(stderr,
                        "ftree: -L must be followed by positive integer\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "--help") == 0) {
            usage();
            exit(0);
        } else if (argv[i][0] != '-') {
            da_push(&dirnames, argv[i]);
        }
    }
    return dirnames;
}

void usage(void)
{
    printf("USAGE:\n"
           "    ftree [options] [directory ...]\n\n"
           "OPTIONS:\n"
           "    -a\n"
           "        Print all files.\n"
           "    -d\n"
           "        Directories only.\n"
           "    -h\n"
           "        Print size of each file in human readable format.\n"
           "    -nc\n"
           "        Turn off colored input.\n"
           "    -s\n"
           "        Print size of each file.\n"
           "    -A\n"
           "        Use ASCII characters for tree structure.\n"
           "    -L level\n"
           "        Set maximum depth of the tree.\n"
           "    --help\n"
           "        Print this help message.\n");
}
