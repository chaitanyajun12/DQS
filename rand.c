#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "request.h"

#define PORT 5000
#define MAX_QUEUE_DELAY 4

int sockfd;
struct sockaddr_in serv_addr;

void connect_to_serv_add()
{
    int portno, n, optval = 1;

    struct hostent * server;

    portno = atoi("5000");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        perror("ERROR ");
    else
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    server = gethostbyname("localhost");
    if(server == NULL)
        perror("NO SUCH HOST ");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        perror("ERROR connecting");
        
}

void write_to_socket(const Request * data)
{
    int size = sendto(sockfd, data, sizeof(Request), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(size != sizeof(Request))
    {
        perror("write ");
        exit(1);
    }

    printf("SUCCESS: data: %d, delay: %d\n", data -> data, data -> delay);
}

// Randomly generate requests and write them into a socket
int main()
{
    int delay;
    srand(time(NULL));

    connect_to_serv_add();

    int data = 0;

    while(1)
    {
        delay = rand() % MAX_QUEUE_DELAY + 1;
        data = data + 1;

        Request req;
        req.delay = delay;
        req.data = data;

        write_to_socket(&req);

        if(data == 1000)
            data = 0;

        sleep(1);
    }

    return 0;
}
