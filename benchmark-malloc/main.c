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
        *buf[i] = 'a'; // force touch
    }
    for (int i = 0; i < ALLOCS; i++) {
        free(buf[i]);
    }
    end = ukplat_monotonic_clock();

    printf("[Malloc] Throughput: %.2f Kops/sec\n", ((double)ALLOCS / ((end - start) / (double)UKPLAT_CLOCK_TICKS_PER_SEC)) / 1000);
    return 0;
}
