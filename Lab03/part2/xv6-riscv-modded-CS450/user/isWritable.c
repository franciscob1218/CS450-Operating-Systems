//Francisco Barba Cuellar #A20121767
//Deimantas Gilys #A20434583
//Nathan Cook #A20458336
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "stdint.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please provide a virtual address.\n");
    exit(1);
  }

  char *va_str = argv[1];
  int va = atoi(va_str);

  int result = isWritable((char*) (intptr_t) va);

  if (result == -1) {
    printf("Error: invalid virtual address.\n");
  } else if (result == 0) {
    printf("The page at virtual address %s is not writable.\n", va_str);
  } else {
    printf("The page at virtual address %s is writable.\n", va_str);
  }

  exit(0);
}