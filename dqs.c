#include "producer.h"
#include "consumer.h"

int main()
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, producer, NULL);
    sleep(1);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
