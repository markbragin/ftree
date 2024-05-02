# ftree

Utility for printing filesystem as a tree.

## Options

- `-a` - print all files
- `-d` - directory only
- `-s` - print the size of each file
- `-h` - print size of file in human readable format
- `-nc` - turn off colored output
- `-A` - use only ASCII characters in output

## Installation

### Using `make`:

`make clean && make release`

### Using `cmake` (+make):

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
make
```

binary will appear in the root directory of the project.


You can use different generator with option `-G`.

### Just compiler

```
gcc -O2 -DNDEBUG src/main.c src/dynamic_array.c src/argparser.c src/tree.c -o ftree
```

## Screenshots

![ftree](./screenshots/example1.jpg "ftree")
