#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bool.h"
#include "colors.h"
#include "dynamic_array.h"
#include "tree.h"

static const char SUBPREFIX_REG[]  = "├── ";
static const char SUBPREFIX_END[]  = "└── ";
static const char SUBPREFIX_PIPE[] = "│   ";

static const char SUBPREFIX_REG_ASCII[]  = "|-- ";
static const char SUBPREFIX_END_ASCII[]  = "'-- ";
static const char SUBPREFIX_PIPE_ASCII[] = "|   ";

static const char SUBPREFIX_EMPTY[] = "    ";

static const char *NO_COLOR   = COLOR_OFF;
static const char *DIR_COLOR  = BLUE_BOLD;
static const char *LINK_COLOR = CYAN_BOLD;
static const char *EXE_COLOR  = GREEN_BOLD;
/* static const char *REG_COLOR  = COLOR_OFF; */

static char CUR_PATH[MAX_PATH_LEN];
static char CUR_PREFIX[MAX_PATH_LEN];

/* Global struct for holding options */
Options OPTIONS;

/* Returns const pointer to filename in given path */
static const char *filename(const char *path);

/* Removes filename in given path */
static void remove_filename(void);

/* Appends filename to given path */
static void append_filename(const char *filename);

/* Prints files in given dir recursively */
static void dirwalk(const char *dirname, unsigned level);

/* Appends subprefix to the end of current prefix */
static void append_prefix(bool last, bool ascii);

/* Removes subprefix from the current prefix */
static void remove_prefix(void);

/* Check if string ends with given substring */
static bool end_with(const char *str, const char *substr);

/* Converts filesize to human readable string */
static void bytes_to_human(long size, char *str);

/* Colorizes string inplace */
static void colorize(char *string, const char *color);

void print_tree(const char *dirname)
{
    strcpy(CUR_PATH, dirname);
    dirwalk(CUR_PATH, 0);
}

static void dirwalk(const char *dirname, unsigned level)
{
    DIR *dirp;
    struct dirent *dir;
    struct stat filestat;
    DynamicArray *files;
    const char *color;
    char size_str[32]           = {0};
    char linkpath[MAX_PATH_LEN] = {0};
    char fname[MAX_PATH_LEN]    = {0};
    int i, rc;

    strcpy(fname, filename(dirname));
    if (!OPTIONS.nocolor) {
        colorize(fname, DIR_COLOR);
    }

    /* Read dir and print its name */
    dirp = opendir(dirname);
    if (!dirp) {
        printf("%s%s [error opening dir]\n", CUR_PREFIX, fname);
        return;
    }
    printf("%s%s\n", CUR_PREFIX, fname);

    /* Return if max depth is set and exceeded */
    if (OPTIONS.depth > 0 && level >= OPTIONS.depth) {
        closedir(dirp);
        return;
    }

    /* Read all filenames and sort them */
    files = da_create(0);
    while ((dir = readdir(dirp)))
        da_push(files, dir->d_name);
    da_sort(files);

    /* Print every filename */
    for (i = 0; i < da_size(files); i++) {
        color = NULL;
        strcpy(fname, da_at(files, i));

        /* Skip . and .. to avoid cycles */
        if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)
            continue;

        /* Skip dotfiles if 'all' option is not set */
        if (fname[0] == '.' && !OPTIONS.all)
            continue;

        append_filename(fname);
        append_prefix(i == da_size(files) - 1, OPTIONS.ascii);

        /* No symlinks in windows */
#if defined(_WIN32) || defined(_WIN64)
        rc = stat(CUR_PATH, &filestat);
#else
        rc = lstat(CUR_PATH, &filestat);
#endif

        if (rc == 0 && S_ISDIR(filestat.st_mode)) {
            dirwalk(CUR_PATH, level + 1);
        } else if (!OPTIONS.dironly) {

            /* Get filesize if options is set */
            if (OPTIONS.human)
                bytes_to_human(filestat.st_size, size_str);
            else if (OPTIONS.size)
                sprintf(size_str, " [%ld]", filestat.st_size);

            /* Set color for executable */
            if (filestat.st_mode & S_IEXEC)
                color = EXE_COLOR;

#if !defined(_WIN32) && !defined(_WIN64)
            /* Set color for symlink and read the content of symlink */
            if (S_ISLNK(filestat.st_mode)) {
                color = LINK_COLOR;
                rc    = readlink(CUR_PATH, linkpath, MAX_PATH_LEN - 1);
                if (rc != -1) {
                    linkpath[rc] = '\0';
                    rc           = stat(CUR_PATH, &filestat);
                    if (rc == 0 && !OPTIONS.nocolor) {
                        if (S_ISDIR(filestat.st_mode))
                            colorize(linkpath, DIR_COLOR);
                        else if (filestat.st_mode & S_IEXEC)
                            colorize(linkpath, EXE_COLOR);
                    }
                }
            }
#endif

            if (!OPTIONS.nocolor && color != NULL)
                colorize(fname, color);

            printf("%s%s%s%s%s\n", CUR_PREFIX, fname,
                   (OPTIONS.human || OPTIONS.size) ? size_str : "",
                   (strlen(linkpath) != 0) ? " -> " : "",
                   (strlen(linkpath) != 0) ? linkpath : "");

            /* Reset linkpath and fname */
            linkpath[0] = '\0';
            fname[0]    = '\0';
        }
        remove_filename();
        remove_prefix();
    }
    closedir(dirp);
    da_destroy(files);
}

static void remove_filename(void)
{
    int i, s;

    for (i = 0, s = 0; CUR_PATH[i] != '\0'; i++)
        if (CUR_PATH[i] == '/')
            s = i;
    CUR_PATH[s] = '\0';
}

static void append_filename(const char *filename)
{
    size_t len;

    len = strlen(CUR_PATH);
    if (len + strlen(filename) + 2 > MAX_PATH_LEN) {
        fprintf(stderr, "Not enough space for prefix\n");
        abort();
    }
    CUR_PATH[len]     = '/';
    CUR_PATH[len + 1] = '\0';
    strcat(CUR_PATH, filename);
}

const char *filename(const char *path)
{
    const char *s;
    int i;

    for (i = 0, s = path; path[i] != '\0'; i++)
        if (path[i] == '/')
            s = &path[i];
    return s == path ? s : s + 1;
}

/* Boilerplace. Fix it later */
static void append_prefix(bool end, bool ascii)
{
    size_t len;

    len = strlen(CUR_PREFIX);
    if (ascii) {
        if (len + sizeof(SUBPREFIX_REG_ASCII) > MAX_PATH_LEN) {
            fprintf(stderr, "Not enough space for prefix\n");
            abort();
        }
        if (end_with(CUR_PREFIX, SUBPREFIX_REG_ASCII)) {
            remove_prefix();
            strcat(&CUR_PREFIX[len - sizeof(SUBPREFIX_REG_ASCII) + 1],
                   SUBPREFIX_PIPE_ASCII);
        } else if (end_with(CUR_PREFIX, SUBPREFIX_END_ASCII)) {
            remove_prefix();
            strcat(&CUR_PREFIX[len - sizeof(SUBPREFIX_END_ASCII) + 1],
                   SUBPREFIX_EMPTY);
        }
        strcat(CUR_PREFIX, end ? SUBPREFIX_END_ASCII : SUBPREFIX_REG_ASCII);
    } else {
        if (len + sizeof(SUBPREFIX_REG) > MAX_PATH_LEN) {
            fprintf(stderr, "Not enough space for prefix\n");
            abort();
        }
        if (end_with(CUR_PREFIX, SUBPREFIX_REG)) {
            remove_prefix();
            strcat(&CUR_PREFIX[len - sizeof(SUBPREFIX_REG) + 1],
                   SUBPREFIX_PIPE);
        } else if (end_with(CUR_PREFIX, SUBPREFIX_END)) {
            remove_prefix();
            strcat(&CUR_PREFIX[len - sizeof(SUBPREFIX_END) + 1],
                   SUBPREFIX_EMPTY);
        }
        strcat(CUR_PREFIX, end ? SUBPREFIX_END : SUBPREFIX_REG);
    }
}

static void remove_prefix(void)
{
    size_t len;

    len = strlen(CUR_PREFIX);
    if (len <= 0)
        return;

    if ((end_with(CUR_PREFIX, SUBPREFIX_REG)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_REG) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_END)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_END) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_PIPE)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_PIPE) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_REG_ASCII)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_REG_ASCII) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_END_ASCII)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_END_ASCII) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_PIPE_ASCII)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_PIPE_ASCII) + 1] = '\0';
    else if ((end_with(CUR_PREFIX, SUBPREFIX_EMPTY)))
        CUR_PREFIX[len - sizeof(SUBPREFIX_EMPTY) + 1] = '\0';
}

static bool end_with(const char *str, const char *substr)
{
    size_t len, sublen;

    len    = strlen(str);
    sublen = strlen(substr);
    if (sublen > len)
        return false;
    return strcmp(&str[len - sublen], substr) == 0;
}

static void bytes_to_human(long size, char *str)
{
    double dsize;

    if ((dsize = (double)size / 1024 / 1024 / 1024) > 1) {
        sprintf(str, " [%.1fG]", dsize);
    } else if ((dsize = (double)size / 1024 / 1024) > 1) {
        sprintf(str, " [%.1fM]", dsize);
    } else if ((dsize = (double)size / 1024) > 1) {
        sprintf(str, " [%.1fK]", dsize);
    } else {
        sprintf(str, " [%ld]", size);
    }
}

static void colorize(char *string, const char *color)
{
    int i, j, new_len, str_len, color_len, nocolor_len;

    str_len     = strlen(string);
    color_len   = strlen(color);
    nocolor_len = strlen(NO_COLOR);

    new_len = color_len + str_len + nocolor_len;
    if (new_len > MAX_PATH_LEN - 1) {
        fprintf(stderr, "Not enough space for color\n");
        return;
    }

    string[new_len] = '\0';
    for (i = new_len - 1, j = nocolor_len - 1; j >= 0; i--, j--)
        string[i] = NO_COLOR[j];
    for (j = str_len - 1; j >= 0; i--, j--)
        string[i] = string[j];
    for (j = color_len - 1; j >= 0; i--, j--)
        string[i] = color[j];

    assert(i == -1);
}
