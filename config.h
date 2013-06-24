#ifndef CONFIG_H_
#define CONFIG_H_

#define DELAY 1;

#define PORT 5000

// Address of all the queues
extern Q_TYPE * queues[10];

// Total queues
extern unsigned int totQueues;

// Queues which have initialized
extern unsigned int init_queues;

// mutex locking on queue length
extern pthread_mutex_t lock;

#endif
