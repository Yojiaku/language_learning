//进程pid: getpid()                 
//线程tid: pthread_self()     //进程内唯一，但是在不同进程则不唯一。
//线程pid: syscall(SYS_gettid)     //系统内是唯一的

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

void *hello()
{
    printf("child, the tid=%lu, pid=%d\n",pthread_self(),syscall(SYS_gettid));
    printf("child, getpid()=%d\n",getpid());
    while(1);
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id,NULL,(void*)hello,NULL);
    printf("parent, getpid()=%d\n",getpid());
    pthread_join(thread_id,NULL);
    return 0;
}
