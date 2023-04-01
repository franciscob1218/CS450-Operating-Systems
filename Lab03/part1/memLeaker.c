#include <stdlib.h>

int main() {
    int *ptr;
    for (int i = 0; i < 10; i++) {
        ptr = malloc(sizeof(int));
    }
    return 0;
}

