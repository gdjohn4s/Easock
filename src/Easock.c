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

int send_msg(int sock, char* hostname, uint16_t port, char* msg)
{
    /* Set a socket structure with host destination information and send a simple message, not a GET/POST etc request */ 
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

void get_req(char *page, char *hostaddr, uint16_t port)
{
    /* 
     * Send a GET request to *page and get response in recvline.
     * the request is creating by prepring the req header with all req options chosen by the user.
     */
    char sendline[1024 + 1], recvline[6000], *ptr;
    int n;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(0);

    int sock = make_sock();

    /* convert IPv4 and IPv6 addresses from text to binary form */
    if (inet_pton(server.sin_family, hostaddr, &server.sin_addr) <= 0)
    {
        printf("Error connecting to raspberry\n");
        exit(1);
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        fprintf(stderr, "Error connecting to %s\n", hostaddr);
        exit(1);
    }

    /* putting function arguments into the request string */
    sprintf (sendline,
             "GET %s HTTP/1.0\r\n"     // Request header
             "Host: %s\r\n"            // Host address
             "Content-type: application/x-www-form-urlencoded\r\n"
             "Content-length: %ld\r\n\r\n"
             "%s\r\n", page, hostaddr, strlen(sendline), sendline);

    printf("Sending request to.. %s\n", hostaddr);
    /* if send it's ok get the response */
    if (send(sock, sendline, strlen(sendline), 0) >= 0)
    {
        while ((n = read(sock, recvline, sizeof(recvline))) > 0)
        {
            recvline[n] = '\0';

            if (fputs(recvline, stdout) == EOF)
            {
                printf("fputs() Error\n");
            }
            /* locating \r\n */
            ptr = strstr(recvline, "\r\n\r\n");
        }
    }
    close(sock);
}
