//Francisco Barba Cuellar #A20121767
//Deimantas Gilys #A20434583
//Nathan Cook #A20458336
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define ADDR_VALID (void*)0x4000
#define ADDR_INVALID (void*)0x80000000

int main(int argc, char *argv[]) {
    // Call whereIs() with a valid virtual address
    int frame_num = whereIs(ADDR_VALID);
    if (frame_num >= 0) {
        fprintf(1, "Physical frame number of %p: %d\n", ADDR_VALID, frame_num);
    } else {
        fprintf(1, "Error: virtual address %p is invalid\n", ADDR_VALID);
    }

    // Call whereIs() with an invalid virtual address
    frame_num = whereIs(ADDR_INVALID);
    if (frame_num >= 0) {
        fprintf(1, "Physical frame number of %p: %d\n", ADDR_INVALID, frame_num);
    } else {
        fprintf(1, "Error: virtual address %p is invalid\n", ADDR_INVALID);
    }

    exit(0);
}

