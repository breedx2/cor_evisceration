
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "sender.h"

int Sender::start() {
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
    }
    int broadcastEnable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port);
    printf("Initializing sender for %s:%d...\n", ip_address, port);
    if (inet_aton(ip_address, &server_addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    return 0;
}

void sendState(WaveState state) {
    printf("sending wave state\n");
}
