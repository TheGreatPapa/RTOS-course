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


void *counterThread(void *threadp)
{
    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=1; i < (threadParams->threadIdx)+1; i++)
        sum=sum+i;
    
    printf("Increment thread idx=%d, gsum=%d\n", threadParams->threadIdx, sum);
		snprintf(log_buffer, 255,"Thread idx=%d, sum[1...%d]=%d\n", 
         threadParams->threadIdx,threadParams->threadIdx, sum);
    custom_log(log_buffer);
    
}


int main (int argc, char *argv[])
{
   int rc;
   int i;

   custom_log(get_uname());

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      (void *)0,     // use default attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
