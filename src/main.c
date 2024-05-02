#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "tree.h"

int main(int argc, char **argv)
{
    struct stat statbuf;

    if (argc == 1) {
        print_tree(".");
    } else {
        if (stat(argv[1], &statbuf) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        } else if (!S_ISDIR(statbuf.st_mode)) {
            fprintf(stderr, "%s is not a directory\n", argv[1]);
            exit(1);
        } else {
            print_tree(argv[1]);
        }
    }
    return 0;
}
