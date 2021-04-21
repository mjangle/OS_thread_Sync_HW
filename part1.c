/**
 * @file part1.c
 * @author Matthew Angle
 * @brief 
 * @version 0.1
 * @date 2021-04-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_OF_LOOPS 1000

struct shared{
    sem_t shared_sem;
    pthread_cond_t thread_cond;
    pthread_mutex_t lock;
    int count;
};


/**
 * @brief prints pong at count
 * 
 * @param count 
 */
void print_ping(int count)
{
    printf("PING: %d\n", count);
}

/**
 * @brief prints pong at count 
 * 
 * @param count 
 */
void print_pong(int count)
{
    printf("PONG: %d\n", count);
}

/**
 * @brief ping function increments shared int signals pong
 * 
 * @param rarg 
 * @return void* 
 */
void * ping(void * rarg)
{   
    struct shared * arg = (struct shared *) rarg;
       for (size_t i = 0; i < NUM_OF_LOOPS; i++)
    {
        //wait for sem to post
        sem_wait(&arg->shared_sem);
        pthread_mutex_lock(&arg->lock);
        arg->count++;
        print_ping(arg->count);
        pthread_mutex_unlock(&arg->lock);
        pthread_cond_signal(&arg->thread_cond);
    }
}

/**
 * @brief pong function, uses sem_post to make sure it's started before ping starts
 * 
 * @param rarg 
 * @return void* 
 */
void * pong(void * rarg)
{
    struct shared * arg = (struct shared *) rarg;

    while(arg->count < NUM_OF_LOOPS){
        //post sem wait for signal
        sem_post(&arg->shared_sem);
        pthread_cond_wait(&arg->thread_cond, &arg->lock);
        print_pong(arg->count);
    }
    
}

int main(int argc, char const *argv[])
{
    /* code */
    //create threads
    pthread_t ping_thread;
    pthread_t pong_thread;
    
    //create shared struct
    struct shared * shared_struct = malloc(sizeof(struct shared));
    shared_struct->count = 0;
    pthread_mutex_init(&shared_struct->lock, NULL);
    sem_init(&shared_struct->shared_sem, 0, 0);
    pthread_cond_init(&shared_struct->thread_cond, NULL);

    //create threads
    pthread_create(&ping_thread, NULL, ping, ( void *) shared_struct);
    pthread_create(&pong_thread, NULL, pong, ( void *) shared_struct);

    //joing threads
    pthread_join(ping_thread, NULL);
    pthread_join(pong_thread, NULL);

    //free struct
    free(shared_struct);

    return 0;
}
