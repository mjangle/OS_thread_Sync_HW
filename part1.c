#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_OF_LOOPS 10

struct shared{
    sem_t shared_sem;
    pthread_cond_t thread_cond;
    pthread_mutex_t lock;
    int count;
};


void print_ping(int count)
{
    printf("PING: %d\n", count);
}

void print_pong(int count)
{
    printf("PONG: %d\n", count);
}

void * ping(void * rarg)
{   
    struct shared * arg = (struct shared *) rarg;
       for (size_t i = 0; i < 10; i++)
    {
        sem_wait(&arg->shared_sem);
        print_ping(arg->count);
        pthread_mutex_lock(&arg->lock);
        arg->count++;
        pthread_mutex_unlock(&arg->lock);
        pthread_cond_signal(&arg->thread_cond);
    }
}

void * pong(void * rarg)
{
    struct shared * arg = (struct shared *) rarg;
    pthread_mutex_lock(&arg->lock);
    sem_post(&arg->shared_sem);
    while(1){
    pthread_cond_wait(&arg->thread_cond, &arg->lock);
    print_pong(arg->count);
    sem_post(&arg->shared_sem);
    }
    pthread_mutex_unlock(&arg->lock);
    sem_post(&arg->shared_sem);
    
}

int main(int argc, char const *argv[])
{
    /* code */
    pthread_t ping_thread;
    pthread_t pong_thread;
    
    sem_t shared_sem;
    pthread_cond_t thread_cond;
    pthread_mutex_t lock;

    sem_init(&shared_sem, 0, 0);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&thread_cond, NULL);
    int count = 1;

    struct shared * shared_struct = malloc(sizeof(struct shared));
    shared_struct->count = count;
    shared_struct->lock = lock;
    shared_struct->shared_sem = shared_sem;
    shared_struct->thread_cond = thread_cond;

    pthread_create(&ping_thread, NULL, ping, ( void *) shared_struct);
    pthread_create(&pong_thread, NULL, pong, ( void *) shared_struct);

    pthread_join(ping_thread, NULL);
    pthread_join(pong_thread, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&thread_cond);

    return 0;
}
