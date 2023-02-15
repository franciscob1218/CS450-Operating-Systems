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

int main(int argc, char *argv[]){
  int fd = 0; // init the file discriptor to zero
  if (argc == 2){ // Check for input from file argument
    fd = open(argv[1], O_RDONLY); // open the file in a read only format
    if (fd < 0){ // check if there is a issue opening the file
      fprintf(stderr, "File %s does not exist\n", argv[1]); //print error msg about opening the file
      exit(1); //exit as a failure
    }
  }

  int chld = open("child.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);  // Create child.txt file in write only permissions and APPEND old file
  int prnt = open("parent.txt", O_CREAT | O_WRONLY | O_APPEND, 0644); // Create parent.txt file in write only permissions and APPEND old file
  if (chld < 0 || prnt < 0){ // check if either file has a issue opening
    perror("Failed to create output files"); // flag error
    exit(1); //exit as a failure
  }

  char buf[512]; // buffer to hold chars
  while (1){ // read input from user or from file and store in buffer
    int n = read(fd, buf, sizeof(buf));
    if (n <= 0){
      break;
    }
    pid_t pid = fork(); // Create a child
    if (pid < 0){ // check if child creation failed
      perror("Failed to fork process"); // flag error
      exit(1); //exit as a failure
    }
    else if (pid == 0){ // is this the child program
      // Child
      write(chld, buf, n); // write from the buffer to the child file 
      exit(0); //exit child with success
    }
    else{
      // Parent
      write(prnt, buf, n); // write from the buffer to the parent file 
      wait(NULL); //wait for child to finish 
    }
  }

  close(chld); // close child file
  close(prnt); // close parent file
  if (fd > 0){ // if and input file was oppened then close it
    close(fd); // close the input file
  }
  exit(0); //exit the program with success
}