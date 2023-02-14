#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[]) {
    int fd = 0;
    if (argc == 2) { // Check for input file argument
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "File %s does not exist\n", argv[1]);
            exit(1);
        }
    }

    int chld = open("child.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); // Create child.txt file
    int prnt = open("parent.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); // Create parent.txt file
    if (chld < 0 || prnt < 0) {
        perror("Failed to create output files");
        exit(1);
    }

    char buf[512];
    while (fgets(buf, sizeof(buf), fd != 0 ? fdopen(fd, "r") : stdin) != NULL) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to fork process");
            exit(1);
        } else if (pid == 0) {
            // Child process
            write(chld, buf, strlen(buf));
            exit(0);
        } else {
            // Parent process
            write(prnt, buf, strlen(buf));
            wait(NULL);
        }
    }

    close(chld);
    close(prnt);
    if (fd > 0) {
        close(fd);
    }
    exit(0);
}