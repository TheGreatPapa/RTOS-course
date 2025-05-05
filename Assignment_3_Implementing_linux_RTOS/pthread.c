#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include "custom_logger.c"

#define NUM_THREADS 128

typedef struct
{
    int threadIdx;
} threadParams_t;

static char log_buffer[255];


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

//Set scheduling policy to FIFO
void set_sched_policy(pthread_attr_t *thread_attr)
{
    struct sched_param sched_param;
    
    sched_param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &sched_param) == -1) {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }

    //Configure Thread attributes
    pthread_attr_init(thread_attr);
    pthread_attr_setinheritsched(thread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(thread_attr, SCHED_FIFO);

    sched_param.sched_priority = 50;
    pthread_attr_setschedparam(thread_attr, &sched_param);

}

void *counterThread(void *threadp)
{
    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=1; i < (threadParams->threadIdx)+1; i++)
        sum=sum+i;
    
    printf("Thread idx=%d, gsum=%d, Running on core:%d\n", threadParams->threadIdx, sum, sched_getcpu());
		snprintf(log_buffer, 255,"Thread idx=%d, sum[1...%d]=%d Running on core:%d\n", 
         threadParams->threadIdx,threadParams->threadIdx, sum,sched_getcpu());
    custom_log(log_buffer);
    
}


int main (int argc, char *argv[])
{
   int i;
   pthread_attr_t attr;
   set_sched_policy(&attr);

   custom_log(get_uname());

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       int ret = pthread_create(
                      &threads[i],   // pointer to thread descriptor
                      &attr,     // use default attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

       if (ret != 0){
            perror("Error Creating Thread");
            exit(EXIT_FAILURE);
        }

   }
   pthread_attr_destroy(&attr);

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

   
   printf("TEST COMPLETE\n");
}
