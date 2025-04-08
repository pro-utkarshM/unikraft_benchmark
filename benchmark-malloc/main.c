#include <stdio.h>
#include <stdlib.h>
#include <uk/time.h>

#define ALLOCS 100000
#define SIZE 256

int main(void) {
    char *buf[ALLOCS];
    uint64_t start, end;

    start = ukplat_monotonic_clock();
    for (int i = 0; i < ALLOCS; i++) {
        buf[i] = malloc(SIZE);
        if (!buf[i]) {
            printf("Allocation failed at %d\n", i);
            return 1;
        }
        *buf[i] = 'a';
    }
    for (int i = 0; i < ALLOCS; i++) {
        free(buf[i]);
    }
    end = ukplat_monotonic_clock();

    double duration = (end - start) / (double)UKPLAT_CLOCK_TICKS_PER_SEC;
    double throughput = ((double)ALLOCS / duration);

    printf("MALLOC_OPS: %.2f\n", throughput);  // For your parser
    return 0;
}
