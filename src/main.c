#include "argparser.h"
#include "dynamic_array.h"
#include "tree.h"

int main(int argc, char **argv)
{
    DynamicArray dirnames;
    int i;

    dirnames = parse_args(argc, argv, &OPTIONS);

    if (dirnames.size == 0) {
        da_push(&dirnames, ".");
    }

    for (i = 0; i < dirnames.size; i++) {
        print_tree(dirnames.items[i]);
    }

    da_destroy(&dirnames);
    return 0;
}
