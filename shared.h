#ifndef SHARED_H_
#define SHARED_H_ 1

#include "config.h"

// Shared data between producer and consumer

Q_TYPE * queues[10];
unsigned int totQueues = 0;
unsigned int init_queues = 0;

#endif
