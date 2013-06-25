#include "consumer.h"

// Setting a timeout event
void set_timeout_event(Q_TYPE * q)
{
	printf("Set timeout event for queue delay: %d\n", q -> delay);
	
	if(q == NULL)
		return;

	struct ev_loop * loop = q -> loop;

	Q_TIME_DATA timer_data;
	timer_data.q = q;
	timer_data.watcher = q -> timer_watcher;

	ev_timer_init (&(timer_data.watcher), timeout_cb, q -> delay, q -> delay);
	ev_timer_start (loop, &(timer_data.watcher));

	ev_run (loop, 0);
}

void read_from_queue(Q_TYPE * q)
{
	int result, data;	
	Request req;	
		
	if((q -> length) > 0)
	{
		result = read(q -> fd[0], &req, sizeof(Request));
		if(result < 0)
		{
		    perror("read ");
		    exit(3);
		}
	
		pthread_mutex_lock(&lock);
		(q -> length) = (q -> length) - 1;
		pthread_mutex_unlock(&lock);

		q -> last_access = time(NULL);

		printf("Consumer: Data: %d, Delay: %d, Length: %d\n", req.data, req.delay, q->length);
	}
}

// Timeout callback
static void timeout_cb (EV_P_ ev_timer *w, int revents)
{
	puts ("timeout");

	Q_TIME_DATA * data = (Q_TIME_DATA *) w;
	Q_TYPE * q = data -> q;	
	
	read_from_queue(q);	
}

/* 
 * Spawn a thread for each of the queues with its own event loop. The timeout repeats itself for
 * delay number of seconds.
 */
void init()
{
	int i;
	
	for(i=0; i<totQueues; i++)
	{
		Q_TYPE * q = queues[i];

		if(!q -> init) 
		{
			printf("QUEUE INITIALIZED: %d\n", q -> delay);
			
			pthread_t t;
			q -> loop = ev_loop_new(EVFLAG_AUTO);
			pthread_create(&t, NULL, &set_timeout_event, (void *) q);
			q -> init = 1;
		}
	
	}
}

void * consumer()
{
	printf("Consumer initiated...\n");
	while(1) {	
			
		if(totQueues > init_queues) 
		{
			init_queues = totQueues;
			init();		
		}	
	
		sleep(3);
	} 
}
