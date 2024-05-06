#include <stdio.h>
#include <unistd.h>

#include "argparser.h"
#include "bool.h"
#include "dynamic_array.h"
#include "tree.h"

int main(int argc, char **argv)
{
    DynamicArray *dirnames;
    size_t i;

    dirnames = parse_args(argc, argv, &OPTIONS);

    if (!isatty(fileno(stdout)))
        OPTIONS.nocolor = true;

    if (da_size(dirnames) == 0)
        da_push(dirnames, ".");

    for (i = 0; i < da_size(dirnames); i++)
        print_tree(da_at(dirnames, i));

    da_destroy(dirnames);
    return 0;
}
