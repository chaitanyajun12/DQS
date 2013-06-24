DQS - Delay based queuing system
================================

DQS has got a producer, multiple queues and a consumer.

1. Producer reads requests from a socket and accomdates then into queues depeding on the delay value associated with each request. 

2. A request is inserted into queue, if delay of request matches the delay of the queue. Otherwise, a new queue is created with the request's delay value.

3. Consumer accesses requests from these queues. If it consumes a request, its got to wait till delay seconds expire. Then only it is supposed the access the same queue again.

Install
=======

1. Open a terminal run,

        sh run_dqs.sh   -   Waits for requests and performs DQS
        
2. Open another terminal run,

        sh rand_req_gen.sh  -  Generates requests randomly and writes them to a queue
        
        
Update
======

The consumption of requests for multiple queues is done! :)

On the way
==========

1. Optimizing DQS.
2. Proper Makefiles.
