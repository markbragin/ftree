# ftree

Utility for printing filesystem as a tree.

## Usage

```
USAGE:
    ftree [options] [directory ...]

OPTIONS:
    -a
        Print all files.
    -d
        Directories only.
    -s
        Print size of each file.
    -h
        Print size of each file in human readable format.
    -nc
        Turn off colored input.
    -A
        Use ASCII characters for tree structure.
    --help
        Print this help message.
```

## Installation

### Using `make`

`make clean && make release`

### Using `cmake` (+make)

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
