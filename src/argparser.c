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
        } else if (argv[i][0] != '-') {
            da_push(&ui.dirnames, argv[i]);
        }
    }
    return ui;
}
