#include "producer.h"
#include "consumer.h"
#include "config.h"

pthread_mutex_t lock;

int main()
{
    pthread_t t1, t2;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex init failed\n");
        return 1;
    }

    pthread_create(&t1, NULL, producer, NULL);
    sleep(1);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}
