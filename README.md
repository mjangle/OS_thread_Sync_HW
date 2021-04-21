## OS Thread Sync HW

The purpose of this homework is to create two threads that will use a shared variable with one thread incrementing the variable. The two threads are ping and pong and will produce output like:

```
Ping: 1
Pong: 1
Ping: 2
Pong: 2
...
Ping: 10
Pong: 10
```

This is achived using p-threads, semaphores, and mutex locks. 
I create a struct to house all shared variable including the count variable that will keep track of the current count, the mutext lock, the semaphore, and the p-thread cond. 

```C
struct shared{
    sem_t shared_sem;
    pthread_cond_t thread_cond;
    pthread_mutex_t lock;
    int count;
};

```

I intilized all the variables and then pass the struct into the threads.

I use a semaphore to signal the ping thread that pong is awake by setting our semapohre to zero

```C
sem_init(&shared_sem, 0, 0);
```


Then inside the ping function I tell the ping function to wait with `sem_wait()` and the thread waits till pong is up and running and pong posts the sem. Pong then waits for a corresponding signal from ping `thread_cond`.

```C
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
```

```C
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
```
