#include "producer.h"
#include "shared.h"

void insert_into_queue(Q_TYPE * q, Request * req)
{
	int result = write(q -> fd[1], req, sizeof(Request));

        if(result < 0)
        {
		perror("write ");
		exit(2);
        }

	(q -> length) = (q -> length) + 1;
	
	printf("Inserted into the queue with data and delay: %d .. %d .. Length: %d\n", req -> data, req -> delay, q -> length);	
}

void create_queue(Request * req)
{
	Q_TYPE * new_q = (Q_TYPE *) malloc(sizeof(Q_TYPE));	

	new_q -> delay = req -> delay;
	new_q -> length = 0;
	new_q -> init = 0;
	new_q -> last_access = 0;
		
	int res = pipe(new_q -> fd);		
	if(res < 0)
	{
		perror("pipe ");
		exit(1);
	}
	
	printf("\n\nNew queue created with delay: %d\n\n", req -> delay);
	totQueues = totQueues + 1;
	queues[totQueues-1] = new_q;

	insert_into_queue(new_q, req);
}

void check_for_queue_n_insert(Request * req)
{
	int i = 0;

	for(i=0; i<totQueues; i++)	// If already queue exists, insert!
	{
		Q_TYPE * q = queues[i];
		if(q -> delay == req -> delay)
		{
			insert_into_queue(q, req);
			break;		
		}
	}

	if(i == totQueues)	// No queue with the same delay, create a queue!
	{		
		create_queue(req);
	}
}

int get_socket_fd()
{
	int optval = 1;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
	
	} else {
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	}

	return sockfd;
}

void bind_fd_to_serv_add(int sockfd)
{
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		perror("ERROR on binding");
}


// Listens for new requests and forwards them to the proper queues.
void listen_for_requests(int sockfd)
{
	int pid, n, newsockfd;
	listen(sockfd, 5);

	struct sockaddr cli_addr;
	int clilen;

	while(1) {

    	clilen = sizeof(cli_addr);
	printf("waiting ..\n");
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		perror("ERROR on accepting connection");

	while(1) 
	{
		Request req;
		n = recvfrom(newsockfd, &req, sizeof(Request), 0, (struct sockaddr *)&cli_addr, &clilen);
		if (n <= 0)
		{
		    perror("ERROR reading from socket");
		    break;
		}
	
		printf("Recv: data: %d, delay: %d\n", req.data, req.delay);
	 
	   	check_for_queue_n_insert(&req);
	}

		sleep(5);
	}

	close(newsockfd);
}

void * producer()
{
	printf("Producer initiated...\n");

	int fd = get_socket_fd();
	bind_fd_to_serv_add(fd);
	listen_for_requests(fd);
}
