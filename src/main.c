#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <netinet/in.h> // for sockaddr_in
#include <string.h>     // for memset()
#include <arpa/inet.h>  // for inet_ntoa()
#include <unistd.h>     // for close()
#include <sys/socket.h>
#include "respond.h"

static char msgBuffer[BUFFER_SIZE];

int main() {
    // listen on server_fd, connection on new_fd
    int server_fd, new_fd;

    // addr information
    struct sockaddr_in myAddr;

    // client addr
    struct sockaddr_in clientAddr;

    // create socket
    // SOCK_STREAM for TCP
    // SOCK_DGRAM for UDP
    if ((server_fd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error in creating socket");
        exit(EXIT_FAILURE);
    }

    // addr family for IP
    myAddr.sin_family = AF_INET;
    // default http port: 80, host_to_network_short()
    myAddr.sin_port = htons(DEFAULT_HTTP_PORT);
    // automatically set server addr with my IP
    myAddr.sin_addr.s_addr = INADDR_ANY;
    // set rest to zero
    memset(&(myAddr.sin_zero), '\0', sizeof(myAddr.sin_zero));

    // bind the addr to the socket
    if (bind(server_fd, (const struct sockaddr*)&myAddr, sizeof(myAddr)) == -1) {
        perror("Error in bind");
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(server_fd, BACKLOG) == -1) {
        perror("Error in listen");
        exit(EXIT_FAILURE);
    }
    else printf("listening on port %d\n", DEFAULT_HTTP_PORT);

    // server part
    while (1) {
        socklen_t sin_size = sizeof(struct sockaddr);
        if ((new_fd=accept(server_fd, (struct sockaddr*)&clientAddr, &sin_size)) == -1) {
            perror("Error in accept");
            exit(EXIT_FAILURE);
        }
        // inet_ntoa() returns a char* for the addr
        printf("Got connection from %s | ", inet_ntoa(clientAddr.sin_addr));

        memset(msgBuffer, '\0', sizeof(msgBuffer));
        if (recv(new_fd, msgBuffer, sizeof(msgBuffer), 0) == SO_ERROR) {
            perror("Error in recv");
            exit(EXIT_FAILURE);
        }
        if (resp(new_fd, msgBuffer) == RESPOND_ERROR) {
            perror("Error in Respond, skipping");
        }
        close(new_fd);
    }

    return 0;
}
