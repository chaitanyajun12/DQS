#ifndef _PRODUCER_H_ 
#define _PRODUCER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ev.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <netdb.h>
#include <netinet/in.h>

#include "qtype.h"
#include "request.h"
#include "config.h"

void insert_into_queue(Q_TYPE * q, Request * req);
void create_queue(Request * req);
void check_for_queue_n_insert(Request * req);
void * producer();
int get_socket_fd();
void bind_fd_to_serv_add(int sockfd);
void listen_for_requests(int sockfd);

#endif
