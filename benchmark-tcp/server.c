#include <uk/netdev.h>
#include <stdio.h>
#include <lwip/sockets.h>
#include <uk/time.h>
#include <string.h>

int main(void) {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char buffer[4096];
    int len;
    uint64_t start, end;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(12345);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 1);
    connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);

    start = ukplat_monotonic_clock();
    while ((len = recv(connfd, buffer, sizeof(buffer), 0)) > 0) {
        // simulate echo
        send(connfd, buffer, len, 0);
    }
    end = ukplat_monotonic_clock();

    double duration = (end - start) / (double)UKPLAT_CLOCK_TICKS_PER_SEC;
    printf("[TCP] Server transfer duration: %.2f seconds\n", duration);
    return 0;
}
