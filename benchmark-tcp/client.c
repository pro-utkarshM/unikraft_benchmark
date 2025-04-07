#include <uk/netdev.h>
#include <stdio.h>
#include <lwip/sockets.h>
#include <uk/time.h>
#include <string.h>

#define SIZE 4096
#define REPS 100000

int main(void) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[SIZE] = "A";
    uint64_t start, end;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("10.0.2.2");
    servaddr.sin_port = htons(12345);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    start = ukplat_monotonic_clock();
    for (int i = 0; i < REPS; i++) {
        send(sockfd, buffer, SIZE, 0);
        recv(sockfd, buffer, SIZE, 0);
    }
    end = ukplat_monotonic_clock();

    double time_sec = (end - start) / (double)UKPLAT_CLOCK_TICKS_PER_SEC;
    double throughput = (double)(SIZE * REPS * 8) / (time_sec * 1e9);
    printf("[TCP] Throughput: %.2f Gbps\n", throughput);
    return 0;
}
