#include<stdio.h>
#include<pthread.h>

void assisthread(void*arg)
{
   printf("I am helping to do some jods\n");
   //sleep(3);
   printf("pthreadID:%lu\n",pthread_self());
   //pthread_exit(0);
}

int main()
{
   pthread_t assistthid;
   int status = 0;
  // pthread_attr_t attr = {0};

  // pthread_attr_init(&attr); // 属社设置
  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   printf("main pthreadID:%lu\n",pthread_self());
  // pthread_create(&assistthid,&attr,(void*)assisthread,NULL);
   pthread_create(&assistthid,NULL,(void*)assisthread,NULL);
   printf("create pthreadID:%lu\n",assistthid);
   pthread_join(assistthid,(void*)&status);//等待线程assisthread结束
   printf("assistthid's exit is caused %d\n",status);
   
   return 0;
}