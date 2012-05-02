#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    void* m = malloc(5);
    printf("Got memory %p\n", m);
    fork();
    return 0;
}
