#ifndef QTYPE_H_
#define QTYPE_H_

#include <time.h>
#include <ev.h>

// Queue descriptor

typedef struct q_type
{
	int delay;
	int fd[2];
	unsigned int length;	
	time_t last_access;
	int init;

	struct ev_loop * loop;
	ev_timer timer_watcher;	

} Q_TYPE;

#endif 
