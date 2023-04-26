#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <ftw.h>

//working for regular computer compilation outside of xv6

// A function to print the name and inode of each entry
int print_entry(const char *file, const struct stat *sb, int flag) {
    printf("%s\t%ld\n", file, sb->st_ino);
    return 0;
}

// A function to walk the directory tree starting from a given directory
void directoryWalker(const char *dir) {
    // Use ftw() to call print_entry() for each entry in the tree
    if (ftw(dir, print_entry, 10) != 0) {
        perror("ftw");
    }
}

int main(int argc, char *argv[]) {
    // Check if a directory is given as an argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s directory\n", argv[0]);
        return 1;
    }
    // Call directoryWalker() with the given directory
    directoryWalker(argv[1]);
    return 0;
}