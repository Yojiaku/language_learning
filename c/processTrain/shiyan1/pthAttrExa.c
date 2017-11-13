/*测试线程属性*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *pthread_func_1();
void *pthread_func_2();

void *pthread_func_1()
{
	int i = 0;
	for(; i<6; i++)
	{
		printf("This is pthread_1.\n");

		if(i == 2)
		{
			pthread_exit(0);
		}
	}
}

void *pthread_func_2()
{
	int i = 0;
	for(; i<3; i++)
	{
		printf("This is pthread_2.\n");
	}
}

int main()
{
	pthread_t pt_1 = 0;
	pthread_t pt_2 = 0;
	pthread_attr_t attr = {0};
	int ret = 0;

	pthread_attr_init(&attr); // 属社设置
	//pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	ret = pthread_create(&pt_1, &attr, pthread_func_1, NULL);
	if(ret != 0)
	{
		printf("创建线程失败\n");
	}

	ret = pthread_create(&pt_2, NULL, pthread_func_2, NULL);
	if(ret != 0)
	{
		printf("创建线程失败\n");
	}

	int ret2;
	ret2 = pthread_join(pt_2, NULL);
	if(ret2 == 0)
		printf("线程2已经结束\n");
	else
		printf("线程2没有结束，错误号%d", ret2);

	int ret1;
	ret1 = pthread_join(pt_1, NULL);
	if(ret1 == 0)
		printf("线程1已经结束\n");
	else
		printf("线程1没有结束，错误号%d\n", ret1);
	return 0;
}