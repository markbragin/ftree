#include "argparser.h"
#include "dynamic_array.h"
#include "tree.h"

int main(int argc, char **argv)
{
    UserInput ui;
    int i;

    ui = parse_args(argc, argv);

    if (ui.dirnames.size == 0) {
        da_push(&ui.dirnames, ".");
    }

    for (i = 0; i < ui.dirnames.size; i++) {
        print_tree(ui.dirnames.items[i], ui.options);
    }

    da_destroy(&ui.dirnames);
    return 0;
}
