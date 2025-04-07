#include <uk/plat/time.h>
#include <uk/print.h>

static uint64_t boot_start_time = 0;

__attribute__((constructor(101))) // runs early during init
static void measure_boot_start(void) {
    boot_start_time = ukplat_monotonic();
    uk_pr_info("[BOOT TIME] Start timestamp: %llu ns\n", boot_start_time);
}

int main(int argc, char *argv[]) {
    uint64_t boot_end_time = ukplat_monotonic();
    uint64_t boot_duration_ns = boot_end_time - boot_start_time;

    uk_pr_info("[BOOT TIME] Reached main()\n");
    uk_pr_info("[BOOT TIME] Duration: %llu ns (%.3f ms)\n", 
               boot_duration_ns, boot_duration_ns / 1e6);

    // your app logic here
    while (1) { } // idle

    return 0;
}
