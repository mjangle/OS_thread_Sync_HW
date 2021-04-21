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
