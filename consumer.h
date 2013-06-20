#ifndef _CONSUMER_H_ 
#define _CONSUMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <ev.h>

#include "qtype.h"
#include "request.h"
#include "config.h"

typedef struct queue_io_data
{
	ev_io watcher;
	Q_TYPE * q;

} Q_IO_DATA;

typedef struct queue_timer_data
{
	ev_timer watcher;
	Q_TYPE * q;

} Q_TIME_DATA;


void set_queue_event(Q_TYPE * q);
void set_timeout_event(Q_TYPE * q);
void read_from_queue(Q_TYPE * q);
static void queue_cb (EV_P_ ev_io *w, int revents);
static void timeout_cb (EV_P_ ev_timer *w, int revents);
int init();
void * consumer();

#endif
