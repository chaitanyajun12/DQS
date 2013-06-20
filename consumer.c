#include "consumer.h"


void set_queue_event(Q_TYPE * q)
{
	printf("Set queue readable event for queue delay: %d\n", q -> delay);

	if(q == NULL)
		return;	

	struct ev_loop * loop = EV_DEFAULT; 

	Q_IO_DATA io_data;
	io_data.q = q;
	io_data.watcher = q -> queue_watcher;
	
	ev_io_init (&(io_data.watcher), queue_cb, q -> fd[0], EV_READ);
	ev_io_start (loop, &(io_data.watcher));

	ev_run (loop, 0);
}

void set_timeout_event(Q_TYPE * q)
{
	printf("Set timeout event for queue delay: %d\n", q -> delay);
	
	if(q == NULL)
		return;

	struct ev_loop * loop = EV_DEFAULT;	

	Q_TIME_DATA timer_data;
	timer_data.q = q;
	timer_data.watcher = q -> timer_watcher;

	ev_timer_init (&(timer_data.watcher), timeout_cb, q -> delay, 0);
	ev_timer_start (loop, &(timer_data.watcher));

	ev_run (loop, 0);
}

void read_from_queue(Q_TYPE * q)
{
	int result, data;	
	Request req;	
	
	result = read(q -> fd[0], &req, sizeof(Request));
        if(result < 0)
        {
            perror("read ");
            exit(3);
        }
	
	(q -> length) = (q -> length) - 1;
	q -> last_access = time(NULL);

        printf("Consumer: Data: %d, Delay: %d, Length: %d\n", req.data, req.delay, q->length);

	if((q -> length) == 0) {
		set_queue_event(q);
	} else {
		set_timeout_event(q);
	}	
}

static void queue_cb (EV_P_ ev_io *w, int revents)
{
	puts ("queue ready");
	ev_io_stop (EV_A_ w);

	Q_IO_DATA * data = (Q_IO_DATA *) w;
	Q_TYPE * q = data -> q;

	time_t curr_time = time(NULL);
	time_t last_access = q -> last_access;
	
	if(((curr_time - last_access) >= (q -> delay)) || last_access == 0)
	{
		read_from_queue(q);
	} else {
		
		sleep(q -> delay);
		set_queue_event(q);
	}	
}

static void timeout_cb (EV_P_ ev_timer *w, int revents)
{
	puts ("timeout");
	ev_timer_stop (EV_A_ w);

	if(totQueues > init_queues)
		init();		
	
	Q_TIME_DATA * data = (Q_TIME_DATA *) w;
	Q_TYPE * q = data -> q;	
	
	read_from_queue(q);	
}

int init()
{
	int i, flag = 0;
	
	for(i=0; i<totQueues; i++)
	{
		Q_TYPE * q = queues[i];

		if(!q -> init) 
		{
			printf("QUEUE INITIALIZED: %d\n", q -> delay);
			
			/* pthread_t t;
			pthread_create(&t, NULL, &set_queue_event, (void *) q); */
			set_queue_event(q);
			q -> init = 1;
		}

		flag = 1;		
	}

	return flag;
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
