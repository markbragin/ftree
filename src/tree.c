#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "bool.h"
#include "colors.h"
#include "dynamic_array.h"
#include "tree.h"

static const char SLASH = '/';

static const char SUBPREFIX_REG[]   = "├── ";
static const char SUBPREFIX_END[]   = "└── ";
static const char SUBPREFIX_PIPE[]  = "│   ";
static const char SUBPREFIX_EMPTY[] = "    ";

static const char *NO_COLOR   = COLOR_OFF;
static const char *REG_COLOR  = COLOR_OFF;
static const char *DIR_COLOR  = BLUE_BOLD;
static const char *LINK_COLOR = CYAN_BOLD;
static const char *EXE_COLOR  = GREEN_BOLD;

static char CUR_PATH[MAX_PATH_LEN];
static char CUR_PREFIX[MAX_PATH_LEN];

/* Returns const pointer to filename in given path */
static const char *filename(const char *path);

/* Removes filename in given path */
static void remove_filename(void);

/* Appends filename to given path */
static void append_filename(const char *filename);

/* Prints files in given dir recursively */
static void dirwalk(const char *dirname);

/* Appends subprefix to the end of current prefix */
static void append_prefix(bool last);

/* Removes subprefix from the current prefix */
static void remove_prefix(void);

/* Check if string ends with given substring */
static bool end_with(const char *str, const char *substr);

void print_tree(const char *dirname)
{
    strcpy(CUR_PATH, dirname);
    dirwalk(CUR_PATH);
}

static void dirwalk(const char *dirname)
{
    DIR *dirp;
    struct dirent *dir;
    struct stat filestat;
    DynamicArray files;
    const char *color;
    int i;

    dirp = opendir(dirname);
    if (!dirp) {
        printf("%s%s%s%s [error opening dir]\n", CUR_PREFIX, DIR_COLOR,
            filename(dirname), NO_COLOR);
        return;
    }
    printf("%s%s%s%s\n", CUR_PREFIX, DIR_COLOR, filename(dirname), NO_COLOR);

    files = da_create(0);
    while ((dir = readdir(dirp)))
        da_push(&files, dir->d_name);

    da_sort(&files);
    for (i = 0; i < files.size; i++) {
        if (strcmp(files.items[i], ".") == 0
            || strcmp(files.items[i], "..") == 0 || files.items[i][0] == '.')
            continue;

        append_filename(files.items[i]);
        append_prefix(i == files.size - 1);

        if (lstat(CUR_PATH, &filestat) == 0 && S_ISDIR(filestat.st_mode)) {
            dirwalk(CUR_PATH);
        } else {
            if (S_ISLNK(filestat.st_mode))
                color = LINK_COLOR;
            else if (filestat.st_mode & S_IXUSR)
                color = EXE_COLOR;
            else
                color = REG_COLOR;
            printf("%s%s%s%s\n", CUR_PREFIX, color, files.items[i], COLOR_OFF);
        }
        remove_filename();
        remove_prefix();
    }
    closedir(dirp);
    da_destroy(&files);
}

static void remove_filename(void)
{
    int i, s;

    for (i = 0, s = 0; CUR_PATH[i] != '\0'; i++)
        if (CUR_PATH[i] == SLASH)
            s = i;
    CUR_PATH[s] = '\0';
}

static void append_filename(const char *filename)
{
    unsigned len;

    len = strlen(CUR_PATH);
    if (len + strlen(filename) + 2 > MAX_PATH_LEN) {
        fprintf(stderr, "Not enough space for prefix\n");
        abort();
    }
    CUR_PATH[len] = SLASH;
    CUR_PATH[len + 1] = '\0';
    strcat(CUR_PATH, filename);
}

const char *filename(const char *path)
{
    const char *s;
    int i;

    for (i = 0, s = path; path[i] != '\0'; i++)
        if (path[i] == SLASH)
            s = &path[i];
    return s == path ? s : s + 1;
}

static void append_prefix(bool end)
{
    unsigned len;

    len = strlen(CUR_PREFIX);
    if (len + sizeof(SUBPREFIX_REG) > MAX_PATH_LEN) {
        fprintf(stderr, "Not enough space for prefix\n");
        abort();
    }
    if (end_with(CUR_PREFIX, SUBPREFIX_REG)) {
        remove_prefix();
        strcat(&CUR_PREFIX[len - sizeof(SUBPREFIX_REG) + 1], SUBPREFIX_PIPE);
    } else if (end_with(CUR_PREFIX, SUBPREFIX_END)) {
        remove_prefix();
        strcat(&CUR_PREFIX[len - sizeof(SUBPREFIX_END) + 1], SUBPREFIX_EMPTY);
    }
    strcat(CUR_PREFIX, end ? SUBPREFIX_END : SUBPREFIX_REG);
}

static void remove_prefix(void)
{
    unsigned len;

    len = strlen(CUR_PREFIX);
    if (len <= 0)
        return;

    if ((end_with(CUR_PREFIX, SUBPREFIX_REG)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_REG) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_END)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_END) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_PIPE)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_PIPE) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_EMPTY)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_EMPTY) + 1] = '\0';
}

static bool end_with(const char *str, const char *substr)
{
    unsigned len, sublen;

    len    = strlen(str);
    sublen = strlen(substr);
    if (sublen > len)
        return false;
    return strcmp(&str[len - sublen], substr) == 0;
}
