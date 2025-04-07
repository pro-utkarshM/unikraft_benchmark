#include <stdio.h>
#include <uk/essentials.h>
#include <uk/syscall.h>
#include <uk/time.h>
#include <uk/assert.h>

int main(void) {
    uint64_t start, end;
    int runs = 100000;

    start = ukplat_monotonic_clock();
    for (int i = 0; i < runs; i++) {
        syscall0(__NR_getpid);
    }
    end = ukplat_monotonic_clock();

    printf("[Syscall Latency] getpid(): %.2f ns\n", (double)(end - start) * 1e9 / runs / UKPLAT_CLOCK_TICKS_PER_SEC);
    return 0;
}

