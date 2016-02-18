#ifndef SENDER_H
#define SENDER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "evis.h"

class Sender {

public:
    Sender(const char *ipAddress, int targetPort){
        ip_address = ipAddress;
        port = targetPort;
    };
    int start();
    void sendState(WaveState);
private:
    const char *ip_address;
    int port;
    int sock;
    struct sockaddr_in server_addr;

    std::string buildMessage(WaveState);
};

#endif
