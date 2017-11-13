/*
 * 首先决定现有的竞争范围并将之赋予PCS
 * 然后生成5个将使用SCS调度方法来运行的独立线程
 */
#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 5

/* Each thread will begin control in this function */
void *runner(void *param)
{
    /* do some work */
    printf("Hello ");
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int i, scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    
    /* get the default attributes */
    pthread_attr_init(&attr);

    /* set the scheduling algorithm to PCS or SCS */
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

    
    /* create the threads */
    for(i=0; i<NUM_THREADS; i++)
        pthread_create(&tid[i], &attr, runner, NULL);

    /* now join on each thread */
    for(i=0; i<NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    /* first inquire on the current scope */
    if(pthread_attr_getscope(&attr, &scope)!=0)
        fprintf(stderr, "Unable to get scheduling scope\n");
    else{
        if(scope==PTHREAD_SCOPE_PROCESS)
            printf("PCS");
        else if(scope == PTHREAD_SCOPE_SYSTEM)
            printf("SCS");
        else
            fprintf(stderr, "Illegal scope value.\n");
        }

}

