#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"
#include "dynamic_array.h"
#include "tree.h"

UserInput parse_args(int argc, char **argv)
{
    int i;
    UserInput ui;

    ui.options  = 0;
    ui.dirnames = da_create(0);

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            ui.options |= T_SIZE;
        } else if (strcmp(argv[i], "-h") == 0) {
            ui.options |= T_HUMAN;
        } else if (strcmp(argv[i], "-nc") == 0) {
            ui.options |= T_NOCOL;
        } else if (strcmp(argv[i], "-a") == 0) {
            ui.options |= T_ALL;
        } else if (strcmp(argv[i], "-d") == 0) {
            ui.options |= T_DIRONLY;
        } else if (strcmp(argv[i], "-A") == 0) {
            ui.options |= T_ASCII;
        } else if (strcmp(argv[i], "--help") == 0) {
            usage();
            exit(0);
        } else if (argv[i][0] != '-') {
            da_push(&ui.dirnames, argv[i]);
        }
    }
    return ui;
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
           "    -s\n"
           "        Print size of each file.\n"
           "    -h\n"
           "        Print size of each file in human readable format.\n"
           "    -nc\n"
           "        Turn off colored input.\n"
           "    -A\n"
           "        Use ASCII characters for tree structure.\n"
           "    --help\n"
           "        Print this help message.\n");
}
