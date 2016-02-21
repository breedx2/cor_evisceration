
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

void Sender::sendState(WaveState state) {
    printf("sending wave state\n");
    std::string message = buildMessage(state);
    int rc = sendto(sock, message.data(), message.length(), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rc == -1) {
        perror("sendto() failed!");
        exit(1);
    }
}

std::string Sender::buildMessage(WaveState state) {
    std::string result = "";
    return
        result.append("player:").append(pointToString(state.getPlayer().pos)).append("\n")
            .append("lives:").append(std::to_string(state.getPlayer().lives)).append("\n")
            .append("score:").append(std::to_string(state.getPlayer().score)).append("\n")
            .append("wave:").append(std::to_string(state.getWave())).append("\n")

            .append("humans:").append(pointListToString(state.getHumans())).append("\n")
            .append("electrodes:").append(pointListToString(state.getElectrodes())).append("\n")
            .append("grunts:").append(pointListToString(state.getGrunts())).append("\n")
            .append("hulks:").append(pointListToString(state.getHulks())).append("\n")
            .append("brains:").append(pointListToString(state.getBrains())).append("\n")
            .append("spheroids:").append(pointListToString(state.getSpheroids())).append("\n")
            .append("enforcers:").append(pointListToString(state.getEnforcers())).append("\n")
            .append("sparks:").append(pointListToString(state.getSparks())).append("\n")
            .append("progs:").append(pointListToString(state.getProgs())).append("\n")
            .append("cruiseMissiles:").append(pointListToString(state.getCruiseMissiles())).append("\n")
            .append("quarks:").append(pointListToString(state.getQuarks())).append("\n")
            .append("tanks:").append(pointListToString(state.getTanks())).append("\n")
            .append("shells:").append(pointListToString(state.getShells())).append("\n");
}

std::string Sender::pointListToString(std::list<Point> points){
    std::string result = "";
    std::list<Point>::iterator iter;
    for (iter = points.begin(); iter != points.end(); ++iter) {
        if(result.length() > 0){
            result.append(";");
        }
        result.append(pointToString(*iter));
    }
    return result;
}

std::string Sender::pointToString(Point point){
    std::string result = "";
    return result.append("(")
        .append( std::to_string(point.x))
        .append(",")
        .append(std::to_string(point.y))
        .append(")");
}
