DQS - Delay based queuing system
================================

DQS has got a producer, multiple queues and a consumer.

Producer reads requests from a socket and accomdates then into queues depeding on the delay value associated with each request. 

A request is inserted into queue, if delay of request matches the delay of the queue. Otherwise, a new queue is created with the request's delay value.

Consumer accesses request. If it consumes a request its got to wait till delay seconds expire. Then only it is supposed the access the queue.

Install
=======

1. Open a terminal run,

        sh run_dqs.sh   -   Waits for requests and performs DQS
        
2. Open another terminal run,

        sh rand_req_gen.sh  -  Generates requests randomly and writes them to a queue
        
        
Status
======

Right now, the producer accomdates the requests to multiple queues. But, the consumer consumes from only a single queue.


On the way
==========

Consumption of data from multiple queues.

