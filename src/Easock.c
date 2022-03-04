#include "Easock.h"


int make_sock()
{
    /* Generate a socket and return it */
    int sock = 0;

    /* Socket creation */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Error creating socket\n");
        exit(-1);
    }

    return sock;
}

/* TODO: extract receive function to make it as standalone
void rcvmsg(int socket, char* received)
{
    ssize_t byte_rcv = recv (socket, received, sizeof(received), 0);
    //ssize_t byte_rcv = read (socket, *received, sizeof(received));
    printf("byte_rcv -> %ld\n", byte_rcv);
    if (byte_rcv > 0) {
        printf("Byte received -> %ld\nMsg received -> %s", byte_rcv, received);
    } else {
        printf("No bytes received\n");
    }

}
*/

int send_msg(int sock, char* hostname, uint16_t port, char* msg)
{
    /* Set a socket structure with host destination information and send a message, then try to receive a response */ 
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(0);

    /* To convert hostname (IP) into network byte ip address */
    if (inet_pton(AF_INET, hostname, &server.sin_addr) <= 0)
    {
        printf("Error converting hostname of %s\n", hostname);
        exit(-1);
    }
    else {
        printf("Trying to connect -> %s\n", hostname);
        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
            // error
            fprintf(stderr, "Error connecting to %s\n", hostname);
            exit(-1);
        }
        else {
            // send message
            char buff[6000];
            if (send(sock, msg, strlen(msg), 0) < 0) {
                fprintf(stderr, "Error sending message to %s\n", hostname);
                exit(-1);
            } else {
                printf("Connected to %s\n", hostname);
                if ( recv( sock, buff, 6000, MSG_DONTWAIT ) < 0 ) {
                    printf("Response not received\n");
                } else {
                    printf( "%s\n", buff );
                }
            }
        }
        return sock;
    }
}
